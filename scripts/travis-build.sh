#!/bin/bash -e

CURRDIR=$PWD
BASEDIR=$(cd "$(dirname "$0")"; pwd)
cd "$BASEDIR/.."

mkdir build
cd build
cmake ..
make
cd ..
./build/sarg_test_c
