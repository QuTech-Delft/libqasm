FROM ubuntu:22.04

RUN apt-get -qq update && \
    apt-get -qq upgrade && \
    apt-get -qq -y install bc build-essential curl git python3 python3-pip swig unzip wget && \
    python3 -m pip install conan pytest

# Install latest emsdk
RUN git clone https://github.com/emscripten-core/emsdk.git && \
    cd emsdk && \
    git pull && \
    ./emsdk install latest && \
    ./emsdk activate latest && \
    cd /

# Install deno
RUN curl -fsSL https://deno.land/install.sh | sh

# Install CMake 3.28
RUN version=3.28 && \
    build=1 && \
    limit=3.20 && \
    result=$(echo "$version >= $limit" | bc -l) && \
    url="https://cmake.org/files/v$version/cmake-$version.$build-linux-x86_64.sh" && \
    mkdir /temp && \
    cd /temp && \
    wget --no-verbose $url && \
    mkdir /opt/cmake && \
    sh cmake-$version.$build-linux-x86_64.sh --prefix=/opt/cmake --skip-license && \
    ln -s /opt/cmake/bin/cmake /usr/local/bin/cmake
   
ADD . /libqasm-copy
WORKDIR /libqasm-copy

# Build emscripten binaries
RUN conan profile detect --force && \
    conan build . -pr=conan/profiles/release-clang-emscripten-wasm -pr:b=conan/profiles/release -b missing && \
    ls -hl build/Release/emscripten

# Rename cqasm_emscripten.js to cqasm_emscripten.mjs
RUN mv build/Release/emscripten/cqasm_emscripten.js build/Release/emscripten/cqasm_emscripten.mjs

# Test emscripten binaries
RUN cd emscripten && \
    /root/.deno/bin/deno run -A test_libqasm.ts \
    cd /
