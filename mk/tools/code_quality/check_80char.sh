#!/usr/bin/env bash

set -e

cd "$(dirname "$0")"/../../..

echo "[===] Checking 80 character limit..."
if grep -rnE ".{81,}" src; then
  exit 1
fi
echo "[===] Checking 80 character limit: done"
