#!/usr/bin/env bash

set -e

cd "$(dirname "$0")"/../../..

echo "[===] Checking copyright headers..."

for file in $(git ls-files src/*/); do
  echo "Checking $file..."
  HEADER="$(
    echo "//  SuperTux Meltdown - Semphris' take on the popular Linux platformer"
    git log --pretty="%an <%ae>" -- $file | sort -u | xargs -d $'\n' bash -c 'for arg do git log --pretty="%ad" --date=format:%Y --author="$arg" '"$file"' | sort -u | sed -n '"'"'1p ; $p'"'"' | tr "\n" "-" | sed "s/\([0-9]\+-\)\1/\1/g;s/.$//"; echo " $arg"; done' _ | sort | sed 's#^#//                #g;1s#^//                #//  Copyright (C) #'
    echo "//"
    echo "//  This program is free software: you can redistribute it and/or modify"
    echo "//  it under the terms of the GNU General Public License as published by"
    echo "//  the Free Software Foundation, either version 3 of the License, or"
    echo "//  (at your option) any later version."
    echo "//"
    echo "//  This program is distributed in the hope that it will be useful,"
    echo "//  but WITHOUT ANY WARRANTY; without even the implied warranty of"
    echo "//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the"
    echo "//  GNU General Public License for more details."
    echo "//"
    echo "//  You should have received a copy of the GNU General Public License"
    echo "//  along with this program.  If not, see <http://www.gnu.org/licenses/>."
    echo ""

    if [ "${file: -4}" = ".hpp" ]; then
      GUARD="$(echo "${file#src/}" | tr '[:lower:]' '[:upper:]' | sed 's#_##g;s#[/.]#_#g')"
      echo "#ifndef HEADER_STM_$GUARD"
      echo "#define HEADER_STM_$GUARD"
    #else
      #INCLUDE="${file%.cpp}"
      #echo "#include \"${INCLUDE#src/}.hpp\""
    fi

    echo ""
  )"

  diff <(echo "$HEADER") <(head -n $(echo "$HEADER" | wc -l) $file)
done

echo "[===] Checking copyright headers: done"
