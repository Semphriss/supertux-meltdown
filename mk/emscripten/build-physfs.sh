#!/usr/bin/env bash

if ! [[ $(basename "$(pwd)") == build* ]]; then
  echo "Not in a build folder, exiting" >&2
  exit 1
fi

git clone https://github.com/icculus/physfs.git physfs

export SDKDIR="$(dirname "$(which emsdk)")"
physfs/extras/buildbot-emscripten.sh
