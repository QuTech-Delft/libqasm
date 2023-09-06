FROM ubuntu:22.04

RUN apt-get -qq update && \
    apt-get -qq upgrade && \
    apt-get -qq -y install build-essential cmake git python3 python3-pip swig && \
    python3 -m pip install conan pytest

ADD . /libqasm

WORKDIR /libqasm
RUN conan profile detect
RUN conan build . -s:h compiler.cppstd=20 -o libqasm/*:build_tests=True -o libqasm/*:compat=True -o libqasm/*:tree_gen_build_tests=True -o libqasm/*:asan_enabled=True -b missing

WORKDIR /libqasm/build/Release
RUN ctest -C Release --output-on-failure

WORKDIR /libqasm
RUN python3 -m pip install .
RUN python3 -m pytest
