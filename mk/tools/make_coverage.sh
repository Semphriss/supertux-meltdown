#!/usr/bin/env bash

set -e

cd "$(dirname "$0")"/../..

for cmd in cmake make lcov genhtml; do
  if ! command -v "$cmd" &> /dev/null; then
    echo "Command '$cmd' missing; cannot run." >&2
    exit 1
  fi
done

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
