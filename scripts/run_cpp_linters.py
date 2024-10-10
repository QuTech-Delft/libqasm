#!/usr/bin/env python3

import os
import subprocess
import sys


def print_usage():
    print("Usage:")
    print("    python3 run_cpp_linters.py <ROOT_FOLDER>")
    print("Where:")
    print("    ROOT_FOLDER: folder containing the .clang-format, .clang-tidy, and C++ files.")
    print("Example:")
    print("    python3 run_cpp_linters.py .")


def get_list_of_cpp_files(root_folder: os.PathLike) -> list[str]:
    ret: list[str] = []
    file_types = ['cpp', 'hpp']
    input_subfolders = ['emscripten', 'include', 'src', 'test']
    for input_subfolder in input_subfolders:
        input_folder = os.path.join(root_folder, input_subfolder)
        for root, dirs, files in os.walk(input_folder):
            for file in files:
                for file_type in file_types:
                    if file.endswith(file_type):
                        ret += [str(os.path.join(root, file))]
    return ret


def run_clang_format(root_folder: os.PathLike):
    print("Running clang-format")
    file_list = get_list_of_cpp_files(root_folder)
    try:
        format_file_path = os.path.join(root_folder, ".clang-format")
        file_list_str = " ".join(file_list)
        command = f"clang-format-18 -i --style=file:{format_file_path} --verbose {file_list_str}"
        subprocess.run(command, shell=True)
    except FileNotFoundError as err:
        print("Error running clang-format: {}".format(err.strerror))
        exit(3)


def run_clang_tidy(root_folder: os.PathLike):
    pass


def run_cpp_linters(root_folder: os.PathLike):
    print("Running C++ linters")
    run_clang_format(root_folder)
    run_clang_tidy(root_folder)


def main(argv):
    if len(argv) != 2:
        print_usage()
        exit(2)
    run_cpp_linters(os.path.abspath(argv[1]))

if __name__ == '__main__':
    main(sys.argv)
