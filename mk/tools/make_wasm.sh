#!/usr/bin/env bash

set -e

cd "$(dirname "$0")"/../..

for cmd in cmake make zip; do
  if ! command -v "$cmd" &> /dev/null; then
    echo "Command '$cmd' missing; cannot run." >&2
    exit 1
  fi
done

if ! command -v "emcmake" &> /dev/null; then
  if [ ! -d emsdk.tmp ]; then
    echo "Emscripten not found, downloading" >&2
    git clone https://github.com/emscripten-core/emsdk.git emsdk.tmp
    cd emsdk.tmp
    ./emsdk install latest
    ./emsdk activate latest
     cd ..
  fi
  source emsdk.tmp/emsdk_env.sh
fi

mkdir -p build.wasm/
cd build.wasm/

../mk/emscripten/build-physfs.sh

emcmake cmake ..
emmake make -j$(nproc)

zip stmeltdown.zip stmeltdown.*
