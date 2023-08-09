#!/bin/bash

if [[ $# -ne 2 ]]; then
    echo "Usage: regenerate_parser.sh <INPUT FOLDER> <OUTPUT FOLDER>"
    echo "Where:"
    echo "    INPUT_FOLDER: folder containing the grammar files."
    echo "    OUTPUT_FOLDER: folder where generated files will be left."
    echo "Example:"
    echo "    regenerate_parser.sh ../src/v3x ../build/Debug/src/v3x"
fi

if ! command -v java &> /dev/null; then
    echo "Please install Java JRE: sudo apt-get update && sudo apt-get -y install default-jre"
    exit 2
fi

input_folder=$1
output_folder=$2

antlr_jar_version="4.13.0"
antlr_jar_filename="antlr-${antlr_jar_version}-complete.jar"
antlr_jar_url="https://www.antlr.org/download/${antlr_jar_filename}"

export ANTLR_HOME="~/.local/bin"
export ANTLR_JAR="${ANTLR_HOME}/${antlr_jar_filename}"
export CLASSPATH=".:${ANTLR_JAR}:${CLASSPATH}"

if [[ ! -f "${ANTLR_JAR}" ]]; then
    wget -P "${ANTLR_HOME}" "${antlr_jar_url}"
fi

set -x  # echo on
java -jar "${ANTLR_JAR}" -Xexact-output-dir -o "${output_folder}" -Dlanguage=Cpp "${input_folder}"/*.g4
