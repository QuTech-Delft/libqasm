#!/usr/bin/env python3

import shutil
import sys


def is_java_jre_installed():
    return shutil.which("java")


def print_usage():
    print("Usage: regenerate_parser.sh <INPUT FOLDER> <OUTPUT FOLDER>")
    print("Where:")
    print("    INPUT_FOLDER: folder containing the grammar files.")
    print("    OUTPUT_FOLDER: folder where generated files will be left.")
    print("Example:")
    print("    generate_antlr_parser.sh ../src/v3x ../build/Debug/src/v3x")


def print_java_jre_installation_instructions():
    print("Please install Java JRE: ")
    print("    Linux:")
    print("        $ apt-get update && apt-get -y install default-jre")
    print("    MacOS:")
    print("        $ brew install java")
    print("    Windows:")
    print("        Download the 'Windows Offline' link from https://www.java.com/en/download/manual.jsp")
    print("        Execute the jre-8version-windows-i586.exe installer")


def generate_antlr_parser(input_folder, output_folder):
    print("Generating ANTLR lexer and parser files...")
    command = "antlr4 -Xexact-output-dir -o \"{:2}\" -Dlanguage=Cpp \"{:1}\"/*.g4"\
        .format(input_folder, output_folder)


def main(argv):
    if len(sys.argv) != 2:
        print_usage()
    if not is_java_jre_installed():
        print_java_jre_installation_instructions()
        exit(2)
    generate_antlr_parser(argv[1], argv[2])


if __name__ == '__main__':
    main()
