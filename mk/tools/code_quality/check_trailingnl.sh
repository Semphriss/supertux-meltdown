#!/usr/bin/env bash

set -e

cd "$(dirname "$0")"/../../..

echo "[===] Checking trailing newlines..."
for file in $(git ls-files src/*); do
  if ! tail -c 2 "$file" | tr '\n' '~' | grep -E "[^~]~" &> /dev/null; then
    echo "File '$file' does not end with exactly one newline" >&2
    exit 1
  fi
done
echo "[===] Checking trailing newlines: done"
