import os
import subprocess
from shutil import copyfile
from sys import platform
from typing import Dict

from setuptools import setup

root_dir = os.path.dirname(os.path.realpath(__file__))
src_dir = os.path.join(root_dir, 'src')
build_dir = os.path.join(src_dir, "cbuild")
libqasm_dir = os.path.join(src_dir, "libQasm")

platforms = {
    'unix': {
        'make_command': 'make',
        'cmake_options': '',
        'clib_name': '_libQasm.so',
        'liblexgram': 'liblexgram.so'
    },
    'darwin': {
        'make_command': 'make',
        'cmake_options': '',
        'clib_name': '_libQasm.so',
        'liblexgram': 'liblexgram.dylib'
    },
    'win32': {
        'make_command': 'mingw32-make',
        'cmake_options': '-G "MinGW Makefiles"',
        'clib_name': '_libQasm.pyd',
        'liblexgram': 'liblexgram.dll'
    }
}


def determine_platform() -> Dict[str, str]:
    """Determine the family of the current platform.

    Based on the system libraries, determine whether the platform is of the UNIX family or the win32 family. Other
    platforms are currently not supported and will raise an exception.
    """
    if platform == "linux" or platform == "linux2":
        return platforms['unix']
    elif platform == "darwin" or platform == "win32":
        return platforms[platform]
    else:
        raise OSError('Platform not recognised!')


def create_directory(directory: str) -> None:
    """Wrapper function for checking whether a directory already exists, and otherwise create it.

    Args:
        directory: the path for the directory that needs to be created.
    """
    if not os.path.exists(directory):
        os.makedirs(directory)


def build_libqasm_library(make_command: str, cmake_options: str) -> None:
    """Call cmake and make to build the c++ libraries.

    Args:
        make_command: the make command to use, varies between windows and unix.
        cmake_options: additional build options to pass to cmake.
    """
    os.chdir(build_dir)
    execute_process(f'git submodule update --init --recursive')
    execute_process(f'cmake {cmake_options} {os.path.join("..", "library")}')
    execute_process(f'{make_command}')
    execute_process(f'{make_command} test')
    os.chdir(root_dir)


def execute_process(command: str) -> None:
    """Execute shell commands.

    Args:
        command: the shell command to execute.
    """
    proc = subprocess.Popen(command, shell=True)
    proc.communicate()


def create_init_file() -> None:
    """Create init file for the libQasm directory

    Create a __init__.py file to make the libQasm directory a python package. This __init__ file will be prepopulated
    with a relative import of libQasm.
    """
    init_file_path = os.path.join(libqasm_dir, '__init__.py')
    with open(init_file_path, 'w') as init_fp:
        init_fp.write('from .libQasm import libQasm')


def copy_file(src_dir: str, dest_dir: str, file_name: str) -> None:
    """Copy a specified file from the source directory to the destination directory.

    Args:
        src_dir: source folder from which to copy the specified file.
        dest_dir: destination folder to which to copy the specified file.
        file_name: the file name of the file to copy.
    """
    copyfile(
        os.path.join(src_dir, file_name),
        os.path.join(dest_dir, file_name)
    )


def build_libqasm():
    """Wrapper that calls the differnt components to build libQasm and place the necessary binaries"""
    sys_platform = determine_platform()
    for directory in [libqasm_dir, build_dir]:
        create_directory(directory)

    build_libqasm_library(sys_platform['make_command'], sys_platform['cmake_options'])
    clibname = sys_platform['clib_name']

    create_init_file()
    copy_file(build_dir, libqasm_dir, clibname)
    copy_file(build_dir, libqasm_dir, "libQasm.py")
    copy_file(build_dir, libqasm_dir, sys_platform['liblexgram'])

    return os.path.join(libqasm_dir, clibname), os.path.join(libqasm_dir, sys_platform['liblexgram'])


clib, liblexgram = build_libqasm()

setup(name='libQasm',
      description='libQasm Python Package',
      author='Kelvin Loh',
      author_email='kel85uk@gmail.com',
      url="https://www.github.com/QE-Lab/libqasm/",
      version='0.0.1',
      python_requires='>=3.6',
      packages=['libQasm'],
      package_dir={'': 'src'},
      package_data={'libQasm': [clib, liblexgram]},
      classifiers=[
          'Development Status :: 3 - Alpha',
          'Programming Language :: Python :: 3',
          'Programming Language :: Python :: 3.6',
          'Programming Language :: Python :: 3.7'],
      license='Other/Proprietary License',
      zip_safe=False)
