#!/usr/bin/env python3

import shutil
import subprocess
import sys


def is_antlr4_installed():
    return shutil.which("antlr4")


def print_usage():
    print("Usage: regenerate_parser.sh <INPUT FOLDER> <OUTPUT FOLDER>")
    print("Where:")
    print("    INPUT_FOLDER: folder containing the grammar files.")
    print("    OUTPUT_FOLDER: folder where generated files will be left.")
    print("Example:")
    print("    generate_antlr_parser.py ../src/v3x ../build/Debug/src/v3x")


def print_antlr4_tools_installation_instructions():
    print("Please install ANTLR4 tools: ")
    print("    pip install antlr4-tools")


def generate_antlr_parser(input_folder, output_folder):
    print("Generating ANTLR lexer and parser files...")
    antlr4 = subprocess.run([
        "antlr4",
        "-Xexact-output-dir",
        "-o",
        "{}".format(output_folder),
        "-Dlanguage=Cpp",
        "{}/cqasm_lexer.g4".format(input_folder),
        "{}/cqasm_parser.g4".format(input_folder)
    ])
    if antlr4.returncode != 0:
        print("Error running antlr4: {}", antlr4.returncode)
        exit(3)


def main(argv):
    if len(argv) != 3:
        print_usage()
        exit(2)
    if not is_antlr4_installed():
        print_antlr4_tools_installation_instructions()
        exit(2)
    generate_antlr_parser(argv[1], argv[2])


if __name__ == '__main__':
    main(sys.argv)
