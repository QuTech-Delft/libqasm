FROM centos:6

RUN yum install -y centos-release-scl && \
    yum install -y devtoolset-7 cmake git

ADD . /src

WORKDIR /build

RUN /usr/bin/scl enable devtoolset-7 "cmake /src -DBUILD_TESTS=ON" && \
    /usr/bin/scl enable devtoolset-7 "make -j" && \
    CTEST_OUTPUT_ON_FAILURE=1 /usr/bin/scl enable devtoolset-7 "make test"
