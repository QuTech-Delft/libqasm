ARG PYTHON_VERSION='3.8'

FROM python:${PYTHON_VERSION}-buster

ENV DEBIAN_FRONTEND=noninteractive
RUN apt update &&\
    apt install -y bison build-essential cmake git swig &&\
    python -m pip install pytest

ADD . /src

WORKDIR /build
RUN cmake /src -DLIBQASM_BUILD_TESTS=ON -DLIBQASM_COMPAT=ON -DTREE_GEN_BUILD_TESTS=ON -DLIBQASM_BUILD_PYTHON=ON
RUN make -j 1
RUN make test CTEST_OUTPUT_ON_FAILURE=TRUE
RUN make install

WORKDIR /src
RUN python -m pip install .
RUN python -m pytest

