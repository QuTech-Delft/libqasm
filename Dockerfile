FROM ubuntu:22.04

RUN apt-get -qq update && \
    apt-get -qq upgrade && \
    apt-get -qq -y install build-essential cmake git flex bison npm python3 python3-pip swig wget && \
    python3 -m pip install conan pytest

ADD . /libqasm-copy

WORKDIR /libqasm-copy

RUN npm install n -g && \
    n stable && \
    ln -sf $(find /usr/local/n/versions/node/ -type f -name node | head -1) /usr/bin/node && \
    nodejs --version

RUN conan profile detect --force && \
    conan build . -pr=conan/profiles/emscripten -pr:b=conan/profiles/release -b missing && \
    ls -hl build/Release/emscripten
