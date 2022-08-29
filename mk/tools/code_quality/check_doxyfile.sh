#!/usr/bin/env bash

set -e

cd "$(dirname "$0")"/../../..

echo "[===] Checking Doxyfile..."
doxygen -s -u Doxyfile
echo "[===] Checking Doxyfile: done"
