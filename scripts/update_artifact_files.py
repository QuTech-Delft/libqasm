#!/usr/bin/env python3

import fileinput
import re

lexer_l_files_list = [
    "src/cqasm-version-lexer.l",
    "src/v1/cqasm-lexer.l",
]
lexer_hpp_files_list = [
    "include/cqasm-version-lexer.hpp",
    "include/v1/cqasm-lexer.hpp",
]
lexer_cpp_files_list = [
    "src/cqasm-version-lexer.cpp",
    "src/v1/cqasm-lexer.cpp",
]
parser_hpp_files_list = [
    "include/cqasm-version-parser.hpp",
    "include/v1/cqasm-parser.hpp",
]
parser_cpp_files_list = [
    "src/cqasm-version-parser.cpp",
    "src/v1/cqasm-parser.cpp",
]
generated_files_list = lexer_hpp_files_list\
                       + lexer_cpp_files_list\
                       + parser_hpp_files_list\
                       + parser_cpp_files_list
files_list = lexer_l_files_list\
             + generated_files_list
lexer_hpp_cpp_files_list = lexer_hpp_files_list\
                           + lexer_cpp_files_list


def fix_tree_gen_cpp_include_path():
    tree_gen_cpp_file_path = "src/tree-gen/tree-all.cpp"
    print("Fixing '{}'".format(tree_gen_cpp_file_path))
    regex = re.compile(r'#include "tree-all.cpp.inc"')
    replacement = r'#include "tree-gen/tree-all.cpp.inc"'
    for line in fileinput.FileInput(tree_gen_cpp_file_path, inplace=True):
        print(regex.sub(replacement, line), end="")


def fix_v1_cqasm_parser_include_path():
    cqasm_parser_hpp_file_path = "src/v1/cqasm-parser.cpp"
    print("Fixing '{}'".format(cqasm_parser_hpp_file_path))
    regex = re.compile(r'#include "cqasm-parser.hpp"')
    replacement = r'#include "v1/cqasm-parser.hpp'
    for line in fileinput.FileInput(cqasm_parser_hpp_file_path, inplace=True):
        print(regex.sub(replacement, line), end="")


# The '%option never-interactive' in the lexer.l files removes the need of using <unistd.h>
# However, flex will still add '#ifndef YY_NO_UNISTD_H; #include <unistd.h>; #endif' blocks in the lexer files
# And that causes Windows builds to fail, since <unistd.h> is not found in Windows
# For the lexer.cpp files, a '#define YY_NO_UNISTD_H 1' is already added via lexer.l
# The function below adds that define to the lexer.hpp files as well
def add_yy_no_unistd_h_defines():
    print("Adding YY_NO_UNISTD_H defines")
    regex = re.compile(r'(#ifndef YY_NO_UNISTD_H)')
    replacement = r'#define YY_NO_UNISTD_H 1\n\1'
    for f in lexer_hpp_files_list:
        print("\t{}".format(f))
        for line in fileinput.FileInput(f, inplace=True):
            print(regex.sub(replacement, line), end="")


# Lexer files define some types, such as INT8_MIN, that are already defined for Windows builds
# The function below adds an '#ifndef _WIN32' guard around those type definitions, preventing compilation warnings
def add_win32_guards():
    print("Adding _WIN32 guards")
    regex = re.compile(r'(/\* Limits of integral types\. \*/)(.*)(#endif /\* ! C99 \*/)', re.DOTALL)
    replacement = r'#ifndef _WIN32\n\n\1\2#endif /* ! _WIN32 */\n\n\3'
    for file_path in lexer_hpp_cpp_files_list:
        print("\t{}".format(file_path))
        with open(file_path, 'r+') as f:
            data = f.read()
            data = regex.sub(replacement, data)
            f.seek(0)
            f.write(data)
            f.truncate()


# Lexer and parser files include comment lines with the format '#line <number> "<path to file>"'
# Where <path to file> will be the root of a local path (e.g. '/home/foo/')
# The problem with this is that generated files depend on the paths of the workspaces from where they are created
# The function below removes tha local root part from the paths
def remove_local_root_paths_from_comments():
    print("Removing local root paths from comments")
    regex = re.compile(r'#line (\d+) ".*(build|libqasm)/(.*)"')
    replacement = r'#line \1 "\3"'
    for f in generated_files_list:
        print("\t{}".format(f))
        for line in fileinput.FileInput(f, inplace=True):
            print(regex.sub(replacement, line), end="")


# Parser files include defines with the format 'YY_CQASM_V<something>_<local root path>_SRC_<file name>_INCLUDED'
# Where <local root path> will be the root of a local path (e.g. '/home/foo/')
# The problem with this is that generated files depend on the paths of the workspaces from where they are created
# The function below removes the local root part from the paths
def remove_local_root_paths_from_defines():
    print("Removing local root paths from defines")
    regex = re.compile(r'(YY_CQASM_V[^_]+).*_BUILD(_SRC.*)')
    replacement = r'\1\2'
    for f in parser_hpp_files_list:
        print("\t{}".format(f))
        for line in fileinput.FileInput(f, inplace=True):
            print(regex.sub(replacement, line), end="")


def main():
    fix_tree_gen_cpp_include_path()
    fix_v1_cqasm_parser_include_path()
    add_yy_no_unistd_h_defines()
    add_win32_guards()
    remove_local_root_paths_from_comments()
    remove_local_root_paths_from_defines()


if __name__ == '__main__':
    main()
