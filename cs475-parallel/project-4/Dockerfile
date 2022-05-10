FROM centos:7

RUN yum -y install make gcc-c++ libgomp

WORKDIR /data
COPY Makefile simd-test.cpp /data/
