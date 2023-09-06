#!/usr/bin/env python3

from urllib.request import urlopen
from urllib import error

import os
import subprocess
import sys
import tempfile


def print_usage():
    print("Usage: generate_antlr_parser.py <INPUT FOLDER> <OUTPUT FOLDER>")
    print("Where:")
    print("    INPUT_FOLDER: folder containing the grammar files.")
    print("    OUTPUT_FOLDER: folder where generated files will be left.")
    print("Example:")
    print("    generate_antlr_parser.py ../src/v3x ../build/Debug/src/v3x")


class AntlrJarFileDownloader:
    file_version = "4.13.0"
    file_name = "antlr4-{}-complete.jar".format(file_version)
    file_path = "{}/{}".format(tempfile.gettempdir(), file_name)

    @classmethod
    def exists_antlr_jar_file(cls):
        return os.path.exists(cls.file_path)

    @classmethod
    def download_antlr_jar_file(cls):
        url = "https://repo1.maven.org/maven2/org/antlr/antlr4/{}/{}".format(cls.file_version, cls.file_name)
        try:
            print("Downloading ANTLR jar file from '{}'".format(url))
            with urlopen(url) as response:
                print("Writing ANTLR jar file to '{}'".format(cls.file_path))
                with open(cls.file_path, "wb") as f:
                    f.write(response.read())
        except error.URLError as err:
            print("Error downloading ANTLR jar file: {}".format(err.strerror))
            exit(3)

    @classmethod
    def get_antlr_jar_file_path(cls):
        if not cls.exists_antlr_jar_file():
            cls.download_antlr_jar_file()
        return cls.file_path


def generate_antlr_parser(input_folder, output_folder, antlr_jar_file_path):
    print("Generating ANTLR lexer and parser files")
    try:
        completed_process = subprocess.run([
            "java",
            "-jar",
            antlr_jar_file_path,
            "-Xexact-output-dir",
            "-o",
            "{}".format(output_folder),
            "-Dlanguage=Cpp",
            "{}/cqasm_lexer.g4".format(input_folder),
            "{}/cqasm_parser.g4".format(input_folder)
        ])
        if completed_process.returncode != 0:
            print("Error generating ANTLR lexer and parser files: {}", completed_process.returncode)
            exit(5)
    except FileNotFoundError as err:
        print("Error running java: {}".format(err.strerror))
        exit(4)


def main(argv):
    if len(argv) != 3:
        print_usage()
        exit(2)
    generate_antlr_parser(argv[1], argv[2], AntlrJarFileDownloader.get_antlr_jar_file_path())


if __name__ == '__main__':
    main(sys.argv)
