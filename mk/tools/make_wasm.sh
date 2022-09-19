#!/usr/bin/env bash

set -e

cd "$(dirname "$0")"/../..

mkdir -p build.wasm/
cd build.wasm/

../mk/emscripten/build-physfs.sh

emcmake cmake ..
emmake make -j$(nproc)

zip stmeltdown.zip stmeltdown.*
