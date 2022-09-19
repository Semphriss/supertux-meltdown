#!/usr/bin/env bash

set -e

cd "$(dirname "$0")"/../..

for cmd in cmake make zip; do
  if ! command -v "$cmd" &> /dev/null; then
    echo "Command '$cmd' missing; cannot run." >&2
    exit 1
  fi
done

for cmd in emcmake emmake; do
  if ! command -v "$cmd" &> /dev/null; then
    echo "No command '$cmd'; did you run `source path/to/emsdk/emsdk_env.sh`?" \
          >&2
    exit 1
  fi
done

mkdir -p build.wasm/
cd build.wasm/

../mk/emscripten/build-physfs.sh

emcmake cmake ..
emmake make -j$(nproc)

zip stmeltdown.zip stmeltdown.*
