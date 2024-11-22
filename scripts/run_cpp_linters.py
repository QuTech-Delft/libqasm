#!/usr/bin/env python3

import logging
import os
import subprocess
import sys


def print_usage():
    usage_str = '''
Usage:
    python3 run_cpp_linters.py <ROOT_FOLDER> [<BUILD_FOLDER>]
Where:
    ROOT_FOLDER: folder containing the .clang-format, .clang-tidy, and C++ files.
    BUILD_FOLDER: build folder (optional, defaulted to 'build/Release').
Example:
    python3 run_cpp_linters.py . cmake-build-debug-clang-wsl
    '''
    logging.info(usage_str)


def get_list_of_cpp_files(root_folder: os.PathLike) -> list[str]:
    ret: list[str] = []
    file_types = ['cpp', 'hpp']
    input_subfolders = ['include', 'src', 'test']
    for input_subfolder in input_subfolders:
        input_folder = os.path.join(root_folder, input_subfolder)
        for root, dirs, files in os.walk(input_folder):
            for file in files:
                for file_type in file_types:
                    if file.endswith(file_type):
                        ret += [str(os.path.join(root, file))]
    return ret


def run_clang_format(root_folder: os.PathLike, file_list_str: str) -> None:
    logging.info("Running clang-format")
    try:
        format_file_path = os.path.join(root_folder, ".clang-format")
        options = f"-i --style=file:{format_file_path} --verbose"
        command = f"clang-format-18 {options} {file_list_str}"
        subprocess.run(command, shell=True)
    except FileNotFoundError as err:
        print("Error running clang-format: {}".format(err.strerror))
        exit(3)


def run_clang_tidy(root_folder: os.PathLike, build_folder: os.PathLike, file_list_str: str) -> None :
    logging.info("Running clang-tidy")
    try:
        config_file_path = os.path.join(root_folder, ".clang-tidy")
        options = f"--config-file={config_file_path} --fix --format-style=file -p {build_folder} --use-color"
        command = f"clang-tidy-18 {options} {file_list_str}"
        subprocess.run(command, shell=True)
    except FileNotFoundError as err:
        print("Error running clang-tidy: {}".format(err.strerror))
        exit(4)


def run_cpp_linters(root_folder: os.PathLike, build_folder: os.PathLike):
    logging.info("Running C++ linters")
    file_list = get_list_of_cpp_files(root_folder)
    file_list_str = " ".join(file_list)
    run_clang_format(root_folder, file_list_str)
    run_clang_tidy(root_folder, build_folder, file_list_str)


def main(argv):
    logging.basicConfig(level=logging.INFO, format='%(message)s')
    if len(argv) < 2 or len(argv) > 3:
        print_usage()
        exit(2)
    root_folder = os.path.abspath(argv[1])
    build_folder = os.path.abspath(argv[2] if len(argv) == 3 else os.path.join("build", "Release"))
    run_cpp_linters(root_folder, build_folder)

if __name__ == '__main__':
    main(sys.argv)
