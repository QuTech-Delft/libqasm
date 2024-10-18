#!/usr/bin/env python3

from urllib.request import urlopen
from urllib import error

import logging
import os
import shutil
import subprocess
import sys
import tempfile


def print_usage():
    usage_str = '''
Usage:
    python3 generate_antlr_parser.py <INPUT_FOLDER> <OUTPUT_SOURCE_FOLDER> <OUTPUT_INCLUDE_FOLDER>
Where:
    INPUT_FOLDER: folder containing the grammar files.
    OUTPUT_SOURCE_FOLDER: folder where generated source files will be left.
    OUTPUT_INCLUDE_FOLDER: folder where generated include files will be left.
Example:
    python3 generate_antlr_parser.py ../src/v3x ../build/Debug/src/v3x ../build/Debug/include/v3x
'''
    logging.info(usage_str)


class AntlrJarFileDownloader:
    file_version = "4.13.1"
    file_name = "antlr4-{}-complete.jar".format(file_version)
    file_path = "{}/{}".format(tempfile.gettempdir(), file_name)

    @classmethod
    def exists_antlr_jar_file(cls):
        return os.path.exists(cls.file_path)

    @classmethod
    def download_antlr_jar_file(cls):
        url = "https://repo1.maven.org/maven2/org/antlr/antlr4/{}/{}".format(cls.file_version, cls.file_name)
        try:
            logging.info("Downloading ANTLR jar file from '{}'".format(url))
            with urlopen(url) as response:
                logging.info("Writing ANTLR jar file to '{}'".format(cls.file_path))
                with open(cls.file_path, "wb") as f:
                    f.write(response.read())
        except error.URLError as err:
            logging.error("Error downloading ANTLR jar file: {}".format(err.strerror))
            exit(3)

    @classmethod
    def get_antlr_jar_file_path(cls):
        if not cls.exists_antlr_jar_file():
            cls.download_antlr_jar_file()
        return cls.file_path


def generate_antlr_parser(input_folder, output_source_folder, output_include_folder, antlr_jar_file_path):
    logging.info("Generating ANTLR lexer and parser files")
    try:
        completed_process = subprocess.run([
            "java",
            "-jar",
            antlr_jar_file_path,
            "-no-listener",
            "-visitor",
            "-Xexact-output-dir",
            "-o",
            "{}".format(output_source_folder),
            "-Dlanguage=Cpp",
            "-Werror",
            "{}/CqasmLexer.g4".format(input_folder),
            "{}/CqasmParser.g4".format(input_folder)
        ])
        if completed_process.returncode != 0:
            logging.error("Error generating ANTLR lexer and parser files: {}", completed_process.returncode)
            exit(5)
        # Move header files to output include folder
        output_files = os.listdir(output_source_folder)
        logging.info(f"Moving output include files to {output_include_folder}")
        for output_file in output_files:
            if output_file.endswith(".h"):
                logging.info(f"\t{output_file}")
                shutil.move(os.path.join(output_source_folder, output_file),
                            os.path.join(output_include_folder, output_file))
    except FileNotFoundError as err:
        logging.error("Error running java: {}".format(err.strerror))
        exit(4)


def main(argv):
    logging.basicConfig(level=logging.INFO, format='%(message)s')
    if len(argv) != 4:
        print_usage()
        exit(2)
    generate_antlr_parser(argv[1], argv[2], argv[3], AntlrJarFileDownloader.get_antlr_jar_file_path())


if __name__ == '__main__':
    main(sys.argv)
