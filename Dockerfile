FROM ubuntu:22.04

RUN apt-get -qq update && \
    apt-get -qq upgrade && \
    apt-get -qq -y install build-essential cmake git python3 python3-pip swig && \
    python3 -m pip install conan pytest

ADD . /libqasm

WORKDIR /libqasm
RUN python3 -m pip install .
RUN python3 -m pytest

WORKDIR /build/Release
RUN ctest -C Release --output-on-failure
