FROM ubuntu:22.04 AS build-stage

ENV LDFLAGS="-Os"
ENV CFLAGS="-Os"
ENV CXXFLAGS="-Os"

RUN apt-get -qq update && \
    apt-get -qq upgrade && \
    apt-get -qq -y install bc build-essential git flex bison python3 python3-pip swig wget && \
    python3 -m pip install conan pytest

# Install latest emsdk
RUN git clone https://github.com/emscripten-core/emsdk.git && \
    cd emsdk && \
    git pull && \
    ./emsdk install latest && \
    ./emsdk activate latest && \
    cd /

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
    conan build . -pr=conan/profiles/emscripten -pr:b=conan/profiles/release -b missing

FROM scratch as export-stage
COPY --from=build-stage /libqasm-copy/build/Release/emscripten/ /
