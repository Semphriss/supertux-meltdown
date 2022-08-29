#!/usr/bin/env bash

set -e

cd "$(dirname "$0")"/../../..

echo "[===] Checking 80 character limit..."
! grep -rnE ".{81,}" src
echo "[===] Checking 80 character limit: done"
