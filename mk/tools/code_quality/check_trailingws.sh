#!/usr/bin/env bash

set -e

cd "$(dirname "$0")"/../../..

echo "[===] Checking trailing whitespace..."
if grep -rnE $'[ \t]$' src; then
  exit 1
fi
echo "[===] Checking trailing whitespace: done"
