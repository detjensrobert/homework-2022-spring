FROM dokken/centos-8

RUN dnf -y install make gcc-c++ libgomp

WORKDIR /data
COPY Makefile simd-test.cpp /data/
