import os
import re
import sys

from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.apple import fix_apple_shared_install_name
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake
from conan.tools.env import VirtualBuildEnv
from conan.tools.files import copy, rm
from conan.tools.scm import Version

from version import get_version

required_conan_version = ">=1.60.0 <2 || >=2.0.6"


class LibqasmConan(ConanFile):
    name = "libqasm"
    version = get_version()
    license = "Apache-2.0"
    homepage = "https://github.com/QuTech-Delft/libqasm"
    url = "https://github.com/conan-io/conan-center-index"
    description = "Library to parse cQASM files"
    topics = ("code generation", "parser", "compiler", "quantum compilation", "quantum simulation")
    settings = "os", "compiler", "build_type", "arch"
    package_type = "library"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "asan_enabled": [True, False],
        "build_python": [True, False],
        "cqasm_python_dir": [None, "ANY"],
        "python_dir": [None, "ANY"],
        "python_ext": [None, "ANY"]
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "asan_enabled": False,
        "build_python": False,
        "cqasm_python_dir": None,
        "python_dir": None,
        "python_ext": None
    }
    exports = "version.py", "include/libqasm/versioning.hpp"
    exports_sources = "CMakeLists.txt", "include/*", "python/*", "res/*", "scripts/*", "src/*", "test/*"

    @property
    def _should_build_test(self):
        return not self.conf.get("tools.build:skip_test", default=True, check_type=bool)

    @property
    def _min_cppstd(self):
        return "20"

    @property
    def _compilers_minimum_version(self):
        return {
            "gcc": "10",
            "clang": "13",
            "apple-clang": "14",
            "Visual Studio": "16",
            "msvc": "192"
        }

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")
        if self.settings.arch == "wasm":
            self.options["antlr4-cppruntime"].shared = self.options.shared

    def layout(self):
        self.folders.source = "."
        self.folders.build = os.path.join("build", str(self.settings.build_type))
        self.folders.generators = os.path.join(self.folders.build, "generators")

        self.cpp.package.libs = ["cqasm"]
        self.cpp.package.includedirs = ["include"]
        self.cpp.package.libdirs = ["lib"]

        self.cpp.source.includedirs = ["include"]
        self.cpp.build.libdirs = ["."]

    def build_requirements(self):
        self.tool_requires("tree-gen/1.0.9")
        if self.settings.arch != "armv8":
            self.tool_requires("zulu-openjdk/21.0.1")
        if self.settings.arch == "wasm":
            self.tool_requires("emsdk/3.1.50")
        if self._should_build_test:
            self.test_requires("gtest/1.15.0")

    def validate(self):
        if self.settings.compiler.cppstd:
            check_min_cppstd(self, self._min_cppstd)
        minimum_version = self._compilers_minimum_version.get(str(self.settings.compiler), False)
        if minimum_version and Version(self.settings.compiler.version) < minimum_version:
            raise ConanInvalidConfiguration(f"{self.ref} requires C++{self._min_cppstd},"
                                            f"which your compiler does not support.")

        if self.settings.arch != "wasm" and self.dependencies["antlr4-cppruntime"].options.shared != self.options.shared:
            raise ConanInvalidConfiguration(f"{self.ref} requires antlr4-cppruntime to be built with the same shared option value.")

    def requirements(self):
        self.requires("fmt/11.0.2", transitive_headers=True)
        self.requires("range-v3/0.12.0", transitive_headers=True)
        self.requires("tree-gen/1.0.9", transitive_headers=True, transitive_libs=True)
        # antlr4-cppruntime/4.13.1 Windows/debug builds from sources started failing with Visual Studio 2022 17.13
        # This code is commented out until a newer Conan package fixes that build
        # if not self.settings.arch == "wasm":
        #     self.requires("antlr4-cppruntime/4.13.1", transitive_headers=True)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.variables["ASAN_ENABLED"] = self.options.asan_enabled
        tc.variables["BUILD_SHARED_LIBS"] = self.options.shared
        tc.variables["LIBQASM_BUILD_EMSCRIPTEN"] = self.settings.arch == "wasm"
        tc.variables["LIBQASM_BUILD_PYTHON"] = self.options.build_python
        tc.variables["LIBQASM_BUILD_TESTS"] = self._should_build_test
        tc.variables["LIBQASM_CQASM_PYTHON_DIR"] = self.options.cqasm_python_dir
        tc.variables["LIBQASM_PYTHON_DIR"] = self.options.python_dir
        tc.variables["LIBQASM_PYTHON_EXT"] = self.options.python_ext
        tc.variables["PYTHON_EXECUTABLE"] = re.escape(sys.executable)
        tc.generate()
        env = VirtualBuildEnv(self)
        env.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        copy(self, "LICENSE.md", src=self.source_folder, dst=os.path.join(self.package_folder, "licenses"))
        cmake = CMake(self)
        cmake.install()
        fix_apple_shared_install_name(self)
        rm(self, "*.pdb", self.package_folder, recursive=True)

    def package_info(self):
        self.cpp_info.libs = ["cqasm"]
