#!/bin/bash -e

CURRDIR=$PWD
BASEDIR=$(cd "$(dirname "$0")"; pwd)
WORK_DIR="$BASEDIR/.."

cd "$WORK_DIR"
mkdir -p dep
cd dep

echo "Getting dependencies"

# Install CTest
if [ -f "ctest.h" ]; then
    echo "-- CTest already installed."
else
    echo -n "-- CTest ..."
    wget -q "https://github.com/bvdberg/ctest/archive/master.zip" -O "ctest.zip"
    unzip -qq "ctest.zip"
    mv "ctest-master/ctest.h" .
    rm -rf "ctest.zip" "ctest-master"
    echo " [OK]"
fi

# Install catch
if [ -f "catch.hpp" ]; then
    echo "-- Catch already installed."
else
    echo -n "-- Catch ..."
    wget -q "https://github.com/philsquared/Catch/archive/V1.5.0.tar.gz" -O "catch.tar.gz"
    tar xzf "catch.tar.gz"
    mv "Catch-1.5.0/single_include/catch.hpp" .
    rm -rf "catch.tar.gz" "Catch-1.5.0"
    echo " [OK]"
fi