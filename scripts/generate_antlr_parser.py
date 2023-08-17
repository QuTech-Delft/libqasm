#!/usr/bin/env python3

import os
import subprocess
import sys


def print_usage():
    print("Usage: regenerate_parser.sh <INPUT FOLDER> <OUTPUT FOLDER>")
    print("Where:")
    print("    INPUT_FOLDER: folder containing the grammar files.")
    print("    OUTPUT_FOLDER: folder where generated files will be left.")
    print("Example:")
    print("    generate_antlr_parser.py ../src/v3x ../build/Debug/src/v3x")


def generate_antlr_parser(input_folder, output_folder):
    print("Generating ANTLR lexer and parser files")
    current_folder_path = os.path.dirname(os.path.abspath(__file__))
    jar_file_path = os.path.join(current_folder_path, "..", "3rd_party", "antlr-4.9.3-complete.jar")
    java = subprocess.run([
        "java",
        "-jar",
        jar_file_path,
        "-Xexact-output-dir",
        "-o",
        "{}".format(output_folder),
        "-Dlanguage=Cpp",
        "{}/cqasm_lexer.g4".format(input_folder),
        "{}/cqasm_parser.g4".format(input_folder)
    ])
    if java.returncode != 0:
        print("Error running java: {}", java.returncode)
        exit(3)


def main(argv):
    if len(argv) != 3:
        print_usage()
        exit(2)
    generate_antlr_parser(argv[1], argv[2])


if __name__ == '__main__':
    main(sys.argv)
