ARG PYTHON_VERSION='3.8'

FROM python:${PYTHON_VERSION}-buster

ENV DEBIAN_FRONTEND=noninteractive
RUN apt update &&\
    apt install -y build-essential cmake bison git

ADD . /src

WORKDIR /build

RUN cmake /src -DLIBQASM_BUILD_TESTS=ON -DLIBQASM_COMPAT=ON -DTREE_GEN_BUILD_TESTS=ON
RUN make -j 1
RUN make test
RUN make install

