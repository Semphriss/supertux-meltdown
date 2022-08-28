#!/usr/bin/env bash

set -e

if ! command -v lcov &> /dev/null || ! command -v genhtml &> /dev/null; then
  echo "lcov not found, not running script" >&2
  exit 1
fi

cd "$(dirname "$0")"/../..

mkdir -p build.cov
rm -rf build.cov
mkdir -p build.cov
cd build.cov
cmake -DCMAKE_CXX_FLAGS="-fprofile-arcs -ftest-coverage" -DCMAKE_EXE_LINKER_FLAGS="-lgcov --coverage" ..
make -j11
lcov --capture --initial --directory CMakeFiles/stmeltdown.dir/ --exclude '/usr/*' --exclude '*/src/main.cpp' --output-file coverage_initial.info
./stmeltdown --test || true
lcov --capture           --directory CMakeFiles/stmeltdown.dir/ --exclude '/usr/*' --exclude '*/src/main.cpp' --exclude 'CompilerIdCXX' --output-file coverage_test.info
lcov --add-tracefile coverage_initial.info --add-tracefile coverage_test.info --output-file coverage_all.info
lcov --remove coverage_all.info '*/tests/*' --output-file coverage.info
mkdir -p coverage
cd coverage
genhtml ../coverage.info --demangle-cpp
