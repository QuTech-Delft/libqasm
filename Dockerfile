FROM centos:6

RUN yum install -y centos-release-scl && \
    yum install -y devtoolset-7 cmake git

ADD . /src

WORKDIR /build

RUN /usr/bin/scl enable devtoolset-7 "cmake /src -DLIBQASM_BUILD_TESTS=ON -DLIBQASM_COMPAT=ON"
RUN /usr/bin/scl enable devtoolset-7 "make -j"
RUN CTEST_OUTPUT_ON_FAILURE=1 /usr/bin/scl enable devtoolset-7 "make test"
RUN CTEST_OUTPUT_ON_FAILURE=1 /usr/bin/scl enable devtoolset-7 "make install"
