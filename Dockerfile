FROM centos:6

RUN yum install -y centos-release-scl && \
    yum install -y devtoolset-7 git wget

# Select CMake version with one of the lines below.
RUN yum install -y cmake
# RUN wget https://github.com/Kitware/CMake/releases/download/v3.12.0/cmake-3.12.0-Linux-x86_64.sh && chmod +x cmake-3.12.0-Linux-x86_64.sh && ./cmake-3.12.0-Linux-x86_64.sh --skip-license
# RUN wget https://cmake.org/files/v2.8/cmake-2.8.11.tar.gz && \
#     tar xvf cmake-2.8.11.tar.gz  && \
#     cd cmake-2.8.11 && \
#     /usr/bin/scl enable devtoolset-7 "./configure" && \
#     /usr/bin/scl enable devtoolset-7 "make -j" && \
#     /usr/bin/scl enable devtoolset-7 "make install"

# Add this line to test CMake logic for when an older version of flex/bison is
# already installed (that is, to make sure it uses the version it built from
# source rather than defaulting to the system version).
# RUN yum install -y flex bison
# RUN wget https://ftp.gnu.org/gnu/m4/m4-1.4.6.tar.gz && \
#     tar xvf m4-1.4.6.tar.gz && \
#     cd m4-1.4.6 && \
#     /usr/bin/scl enable devtoolset-7 "./configure" && \
#     /usr/bin/scl enable devtoolset-7 "make -j" && \
#     /usr/bin/scl enable devtoolset-7 "make install"
# RUN wget https://ftp.gnu.org/gnu/bison/bison-3.0.tar.gz && \
#     tar xvf bison-3.0.tar.gz && \
#     cd bison-3.0 && \
#     /usr/bin/scl enable devtoolset-7 "./configure" && \
#     /usr/bin/scl enable devtoolset-7 "make -j" && \
#     /usr/bin/scl enable devtoolset-7 "make install"
# RUN wget https://github.com/westes/flex/releases/download/v2.6.1/flex-2.6.1.tar.gz && \
#     tar xvf flex-2.6.1.tar.gz && \
#     cd flex-2.6.1 && \
#     /usr/bin/scl enable devtoolset-7 "./configure" && \
#     /usr/bin/scl enable devtoolset-7 "make -j" && \
#     /usr/bin/scl enable devtoolset-7 "make install"


ADD . /src

WORKDIR /build

RUN /usr/bin/scl enable devtoolset-7 "cmake /src -DLIBQASM_BUILD_TESTS=ON -DLIBQASM_COMPAT=ON -DTREE_GEN_BUILD_TESTS=ON"
RUN /usr/bin/scl enable devtoolset-7 "make -j"
RUN CTEST_OUTPUT_ON_FAILURE=1 /usr/bin/scl enable devtoolset-7 "make test"
RUN CTEST_OUTPUT_ON_FAILURE=1 /usr/bin/scl enable devtoolset-7 "make install"
