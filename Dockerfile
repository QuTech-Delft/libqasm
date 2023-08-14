FROM ubuntu:22.04

RUN apt-get -qq update && \
    apt-get -qq upgrade && \
    apt-get -qq -y install flex bison m4 build-essential cmake git swig python3 python3-pip && \
    python3 -m pip install --upgrade pip pytest antlr4-tools && \
    echo Y | antlr4 -v 4.9.3

ADD . /libqasm

WORKDIR /build
RUN cmake /libqasm -DLIBQASM_BUILD_TESTS=ON -DLIBQASM_COMPAT=ON -DTREE_GEN_BUILD_TESTS=ON -DLIBQASM_BUILD_PYTHON=ON
RUN make -j 1
RUN make test CTEST_OUTPUT_ON_FAILURE=TRUE
RUN make install

WORKDIR /libqasm
RUN python3 -m pip install .
RUN python3 -m pytest
