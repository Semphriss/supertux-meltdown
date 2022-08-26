#!/usr/bin/env bash

set -e

cd "$(dirname "$0")"/../..

VERSION="$(git describe --tags --always)"

if ! echo "$VERSION" | grep -xiE "[a-z0-9\\._\\-]+" &> /dev/null; then
  echo "Unsafe version string '$VERSION', quitting." >&2
  exit 1
fi

sed "s/\\(PROJECT_NUMBER *=\\).*/\\1 $VERSION/g" Doxyfile | doxygen -
