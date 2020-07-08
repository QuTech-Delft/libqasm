FROM centos:6

RUN yum install -y centos-release-scl && \
    yum install -y devtoolset-7 cmake git

# Add this line to test CMake logic for when an older version of flex/bison is
# already installed (that is, to make sure it uses the version it built from
# source rather than defaulting to the system version).
#RUN yum install -y flex bison

ADD . /src

WORKDIR /build

RUN /usr/bin/scl enable devtoolset-7 "cmake /src -DLIBQASM_BUILD_TESTS=ON -DLIBQASM_COMPAT=ON"
RUN /usr/bin/scl enable devtoolset-7 "make -j"
RUN CTEST_OUTPUT_ON_FAILURE=1 /usr/bin/scl enable devtoolset-7 "make test"
RUN CTEST_OUTPUT_ON_FAILURE=1 /usr/bin/scl enable devtoolset-7 "make install"
