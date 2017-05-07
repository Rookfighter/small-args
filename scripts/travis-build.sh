#!/bin/bash -e

CURRDIR=$PWD
BASEDIR=$(cd "$(dirname "$0")"; pwd)
cd "$BASEDIR/.."

mkdir build
cd build
cmake ..
make
./sarg_test_c
