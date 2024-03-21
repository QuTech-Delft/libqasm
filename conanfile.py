import os
import re
import sys

from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake
from conan.tools.env import VirtualBuildEnv
from conan.tools.files import copy
from conan.tools.scm import Version

from version import get_version


class LibqasmConan(ConanFile):
    name = "libqasm"
    version = get_version()

    # Optional metadata
    license = "Apache-2.0"
    homepage = "https://github.com/QuTech-Delft/libqasm"
    url = "https://github.com/conan-io/conan-center-index"
    description = "Library to parse cQASM files"
    topics = ("code generation", "parser", "compiler", "quantum compilation", "quantum simulation")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "shared": [True, False],
        "fPIC": [True, False],
        "asan_enabled": [True, False],
        "build_python": [True, False],
        "build_tests": [True, False],
        "compat": [True, False],
        "cqasm_python_dir": [None, "ANY"],
        "python_dir": [None, "ANY"],
        "python_ext": [None, "ANY"],
        "tree_gen_build_tests": [True, False]
    }
    default_options = {
        "shared": False,
        "fPIC": True,
        "asan_enabled": False,
        "build_python": False,
        "build_tests": False,
        "compat": False,
        "cqasm_python_dir": None,
        "python_dir": None,
        "python_ext": None,
        "tree_gen_build_tests": False
    }

    exports = "version.py", "include/version.hpp"
    exports_sources = "CMakeLists.txt", "include/*", "python/*", "res/*", "scripts/*", "src/*", "test/*"

    def build_requirements(self):
        self.tool_requires("m4/1.4.19")
        self.tool_requires("tree-gen/1.0.7")
        if self.settings.os == "Windows":
            self.tool_requires("winflexbison/2.5.24")
        else:
            self.tool_requires("flex/2.6.4")
            self.tool_requires("bison/3.8.2")
        self.tool_requires("zulu-openjdk/11.0.19")
        if self.settings.arch == "wasm":
            self.tool_requires("emsdk/3.1.50")
        if self.options.build_tests:
            self.test_requires("gtest/1.14.0")

    def requirements(self):
        self.requires("fmt/10.2.1")
        self.requires("range-v3/0.12.0")
        self.requires("tree-gen/1.0.7")
        if not self.settings.arch == "wasm":
            self.requires("antlr4-cppruntime/4.13.1")

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def configure(self):
        if self.options.shared:
            self.options.rm_safe("fPIC")

    def layout(self):
        self.folders.source = "."
        self.folders.build = os.path.join("build", str(self.settings.build_type))
        self.folders.generators = os.path.join(self.folders.build, "generators")

        self.cpp.package.libs = ["cqasm"]
        self.cpp.package.includedirs = ["include"]
        self.cpp.package.libdirs = ["lib"]

        self.cpp.source.includedirs = ["include"]
        self.cpp.build.libdirs = ["."]

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.variables["ASAN_ENABLED"] = self.options.asan_enabled
        tc.variables["LIBQASM_BUILD_EMSCRIPTEN"] = self.settings.arch == "wasm"
        tc.variables["LIBQASM_BUILD_PYTHON"] = self.options.build_python
        tc.variables["LIBQASM_BUILD_TESTS"] = self.options.build_tests
        tc.variables["LIBQASM_COMPAT"] = self.options.compat
        tc.variables["LIBQASM_CQASM_PYTHON_DIR"] = self.options.cqasm_python_dir
        tc.variables["LIBQASM_PYTHON_DIR"] = self.options.python_dir
        tc.variables["LIBQASM_PYTHON_EXT"] = self.options.python_ext
        tc.variables["PYTHON_EXECUTABLE"] = re.escape(sys.executable)
        tc.variables["TREE_GEN_BUILD_TESTS"] = self.options.tree_gen_build_tests
        tc.generate()
        env = VirtualBuildEnv(self)
        env.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def validate(self):
        compiler = self.settings.compiler
        version = Version(self.settings.compiler.version)
        if compiler == "apple-clang":
            if version < "14":
                raise ConanInvalidConfiguration("libqasm requires at least apple-clang++ 14")
        elif compiler == "clang":
            if version < "13":
                raise ConanInvalidConfiguration("libqasm requires at least clang++ 13")
        elif compiler == "gcc":
            if version < "10.0":
                raise ConanInvalidConfiguration("libqasm requires at least g++ 10.0")
        elif compiler == "msvc":
            if version < "19.29":
                raise ConanInvalidConfiguration("libqasm requires at least msvc 19.29")
        else:
            raise ConanInvalidConfiguration("Unsupported compiler")
        if compiler.get_safe("cppstd"):
            check_min_cppstd(self, "20")

    def package(self):
        copy(self, "LICENSE.md", src=self.source_folder, dst=os.path.join(self.package_folder, "licenses"))
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["cqasm"]
