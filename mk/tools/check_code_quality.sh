#!/usr/bin/env bash

set -e

cd "$(dirname "$0")"

for script in code_quality/*; do
  $script
done

echo -e "\n\nCheck ran successfully, everything should be clean"
