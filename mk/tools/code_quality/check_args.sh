#!/usr/bin/env bash

set -e

cd "$(dirname "$0")"/../../..

echo "[===] Checking arguments..."

# Not a perfect way, it's possible to make an argument slip through this check.
# TODO: improve this script's efficiency

# Actual compiled arguments
OPTS="$(grep -oE "arg *== *\"--.*\"" src/game/game_manager.cpp                 \
        | sed 's#arg *== *\"--\(.*\)\"#\1#g')"

echo "# Output of --help"
grep -oE "<< \"  -., --[^ ]+" src/game/game_manager.cpp                        \
  | sed 's#.\+-\(.\+\)$#\1#g' | diff <(echo "$OPTS") -

echo "# Man pages"
grep -E "^.B \\\\-" mk/unix/stmeltdown.6                                       \
  | sed 's#.*\\-\\-\([^ ]\+\).*#\1#g' | diff <(echo "$OPTS") -

echo "# Bash completion"
grep -xE " *STM_ARGS\+=\('--.*'\)" mk/unix/bash_completion.sh                  \
  | sed "s# *STM_ARGS+=('--\(.*\)')#\1#g" | diff <(echo "$OPTS") -

# Add here checks for wherever arguments may need to be listed in the future.

echo "[===] Checking arguments: done"
