#!/usr/bin/env bash

parse=(src tests tools docs CHANGELOG.md COMPILING.md CONTRIBUTING.md LICENSE \
       README.md ROADMAP.md .gitignore)
ignore=(data external mk clickable.json .gitmodules Doxyfile CMakeLists.txt)

if ! ls src/ > /dev/null 2>&1; then
  echo "Please set your current directory to the root of the repository."
  exit 1
fi

RETURN_CODE=0
for entry in $(git ls-tree HEAD --name-only); do
  if printf '%s\0' "${parse[@]}" | grep -Fqxz "$entry"; then
    if ! grep -HErn ".{81,}" $entry; then
      RETURN_CODE=1
    fi
  elif ! printf '%s\0' "${ignore[@]}" | grep -Fqxz "$entry"; then
    echo "Entry '$entry' is neither listed as parse nor ignore, please fix.";
    exit 1
  fi
done

exit $RETURN_CODE
