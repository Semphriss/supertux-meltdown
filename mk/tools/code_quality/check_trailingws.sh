#!/usr/bin/env bash

set -e

cd "$(dirname "$0")"/../../..

echo "[===] Checking trailing whitespace..."
! grep -rnE $'[ \t]$' src
echo "[===] Checking trailing whitespace: done"
