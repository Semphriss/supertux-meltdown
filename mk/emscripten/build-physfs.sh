#!/usr/bin/env bash

set -e

if ! [[ $(basename "$(pwd)") == build* ]]; then
  echo "Not in a build folder, exiting" >&2
  exit 1
fi

for cmd in emcmake emmake cmake make git; do
  if ! command -v "$cmd" &> /dev/null; then
    echo "Command '$cmd' missing; cannot run." >&2
    exit 1
  fi
done

if [ ! -d physfs ]; then
  git clone https://github.com/icculus/physfs.git physfs
fi

mkdir -p physfs/build.wasm
cd physfs/build.wasm
emcmake cmake -DPHYSFS_BUILD_SHARED=OFF ..
emmake make 
