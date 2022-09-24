#!/usr/bin/env bash

set -e

cd "$(dirname "$0")"/../../..

echo "[===] Checking #include's..."

# Works around #ifdef's properly
for file in $(git ls-files src/*/); do
  echo "Checking $file..."

  # `sed 's/"[a-z_/]+.cpp"//g')`: Header files declaring template functions need
  # to #include the corresponding body. Thise should be ignored here.
  includes="$(grep -E "^(#include|$)" "$file" | sed 's/#include //g'           \
        | sed 's/"[a-z_/]\+\.cpp"//g' | tr '\n' '~' | sed 's/~\+$//g;s/~~/#/g')"

  if [ "$includes" = "" ]; then
    # No include at all is allowed in all source files
    continue
  fi

  if [ "${file: -4}" = ".cpp" ]; then
    if [ "${file:0:10}" = "src/tests/" ]; then
      # TODO: Validate test files
      # For now, test files are excluded from validation due to their structure.
      continue
    fi

    numfields=$(($(echo "$includes" | tr -dc '#' | wc -c) + 1))
    fields="${includes:1}"
    currnum=2
    curr="$(echo "$fields" | cut -d '#' -f $currnum -s)"

    include_guard="$(echo "${file:4:-4}.hpp" | tr '[:lower:]' '[:upper:]' |    \
                     sed 's#_##g;s#[/.]#_#g')"
    if grep "$include_guard" "$file" &> /dev/null; then
      # Files with template functions use some hacky tricks to compile properly
      if ! [[ "$includes" =~ ^\#[^\#~] ]]; then
        echo "File '$file' must have exactly two empty lines before headers:"
              "one above and one below the include guard." >&2
        exit 1
      fi
      currnum=3
    else
      if ! [[ "$includes" =~ ^~[^\#~] ]]; then
        echo "File '$file' must have exactly one empty line before headers." >&2
        exit 1
      fi
    fi

    if ! [ "$(echo "$fields" | cut -d '#' -f 1)" = \"${file:4:-4}.hpp\" ]; then
      echo "File '$file' must start with its parent header file in a single"   \
           "block, between quotes, separated above and below by exactly one"   \
           "empty line." >&2
      exit 1
    fi

    # System includes
    if [[ "${curr//\~/}" =~ ^(\<[a-z_]+\>)+$ ]]; then
      # TODO: validate that those are system header files

      header_list="$(echo "${curr:1:-1}" | sed 's/>~</~/g' | tr '~' '\n')"
      sorted_list="$(echo "$header_list" | sort)"

      if ! [ "$header_list" = "$sorted_list" ]; then
        echo "File '$file' does not have system headers sorted:" >&2
        diff <(echo "$header_list") <(echo "$sorted_list")
        exit 1
      fi

      currnum=$(($currnum + 1))
      curr="$(echo "$fields" | cut -d '#' -f $currnum -s)"
    fi

    # Dependency includes
    if [ ! "$curr" = "" ] && [[ "${curr//\~/}" =~ ^(\"emscripten\.h\")?(\"physfs\.h\")?(\"SDL2/SDL\.h\")?(\"SDL2/SDL_image\.h\")?(\"SDL2/SDL_ttf\.h\")?$ ]]; then
      currnum=$(($currnum + 1))
      curr="$(echo "$fields" | cut -d '#' -f $currnum -s)"
    fi

    # Local includes
    if [[ "${curr//\~/}" =~ ^(\"[a-z]+/[a-z_/]+\.hpp\")+$ ]]; then
      header_list="$(echo "${curr:1:-1}" | sed 's/"//g' | tr '~' '\n')"
      sorted_list="$(echo "$header_list" | sort)"

      if ! [ "$header_list" = "$sorted_list" ]; then
        echo "File '$file' does not have local headers sorted:" >&2
        diff <(echo "$header_list") <(echo "$sorted_list")
        exit 1
      fi

      for header in $header_list; do
        if ! [ -f src/$header ]; then
          echo "File '$file': Header '$header' is not a local file." >&2
          exit 1
        fi
      done

      currnum=$(($currnum + 1))
      curr="$(echo "$fields" | cut -d '#' -f $currnum -s)"
    fi

    if ! [ $currnum = $(($numfields + 1)) ]; then
      echo "File '$file' has unrecognized header block #$currnum. Please make" \
           "sure the blocks are, in order: system, dependencies, and local." >&2
      exit 1
    fi
  elif [ "${file: -4}" = ".hpp" ]; then
    if ! [[ "$includes" =~ ^\#[^\#~] ]]; then
      echo "File '$file' must have exactly two empty lines before headers (one"\
           "before the include guard, and one after)" >&2
      exit 1
    fi

    numfields=$(($(echo "$includes" | tr -dc '#' | wc -c) + 1))
    fields="$includes"

    currnum=2
    curr="$(echo "$fields" | cut -d '#' -f $currnum -s)"

    # Inheritance includes
    if [[ "${curr//\~/}" =~ ^(\"[a-z]+/[a-z_/]+\.hpp\")+$ ]]; then
      header_list="$(echo "${curr:1:-1}" | sed 's/"//g' | tr '~' '\n')"
      sorted_list="$(echo "$header_list" | sort)"

      if ! [ "$header_list" = "$sorted_list" ]; then
        echo "File '$file' does not have inheritance/local headers sorted:" >&2
        diff <(echo "$header_list") <(echo "$sorted_list")
        exit 1
      fi

      for header in $header_list; do
        if ! [ -f src/$header ]; then
          echo "File '$file': Header '$header' is not a local file." >&2
          exit 1
        fi
      done

      currnum=$(($currnum + 1))
      curr="$(echo "$fields" | cut -d '#' -f $currnum -s)"
    fi

    # System includes
    if [[ "${curr//\~/}" =~ ^(\<[a-z_]+\>)+$ ]]; then
      # TODO: validate that those are system header files

      header_list="$(echo "${curr:1:-1}" | sed 's/>~</~/g' | tr '~' '\n')"
      sorted_list="$(echo "$header_list" | sort)"

      if ! [ "$header_list" = "$sorted_list" ]; then
        echo "File '$file' does not have system headers sorted:" >&2
        diff <(echo "$header_list") <(echo "$sorted_list")
        exit 1
      fi

      currnum=$(($currnum + 1))
      curr="$(echo "$fields" | cut -d '#' -f $currnum -s)"
    fi

    # Dependency includes
    if [ ! "$curr" = "" ] && [[ "${curr//\~/}" =~ ^(\"emscripten\.h\")?(\"physfs\.h\")?(\"SDL2/SDL\.h\")?(\"SDL2/SDL_image\.h\")?(\"SDL2/SDL_ttf\.h\")?$ ]]; then
      currnum=$(($currnum + 1))
      curr="$(echo "$fields" | cut -d '#' -f $currnum -s)"
    fi

    # Local includes
    if [[ "${curr//\~/}" =~ ^(\"[a-z]+/[a-z_/]+\.hpp\")+$ ]]; then
      header_list="$(echo "${curr:1:-1}" | sed 's/"//g' | tr '~' '\n')"
      sorted_list="$(echo "$header_list" | sort)"

      if ! [ "$header_list" = "$sorted_list" ]; then
        echo "File '$file' does not have local headers sorted:" >&2
        diff <(echo "$header_list") <(echo "$sorted_list")
        exit 1
      fi

      for header in $header_list; do
        if ! [ -f src/$header ]; then
          echo "File '$file': Header '$header' is not a local file." >&2
          exit 1
        fi
      done

      currnum=$(($currnum + 1))
      curr="$(echo "$fields" | cut -d '#' -f $currnum -s)"
    fi

    if ! [ $currnum = $(($numfields + 1)) ]; then
      echo "File '$file' has unrecognized header block #$(($currnum - 1))."    \
           "Please make sure the blocks are, in order: inheritance, system,"   \
           "dependencies, and local." >&2
      exit 1
    fi
  fi

#  # Note: for easy parsing, all newlines are replaced with ~'s below
#  if [ "${file: -4}" = ".cpp" ]; then
#    REGEX_FRONT="~\"${file:4:-4}\\.hpp\"~"
#  elif [ "${file: -4}" = ".hpp" ]; then
#    # Headers have one extra leading \n before the include guard (#ifndef ...)
#    REGEX_FRONT='(~("[a-zA-Z0-9_/]+\.hpp"~)+)?'
#  else
#    echo "Can't parse file '$file': neither .cpp not .hpp" >&2
#    exit 1
#  fi
#
#  #grep -E "^(#include|$)" "$file" | sed 's/#include //g' | tr '\n' '~'; echo
#  #echo "$REGEX_FRONT"'(~(<[a-z_]+>~)+)?(~(("physfs.h"|"SDL2/SDL.h"|"SDL2/SDL_image.h"|"SDL2/SDL_ttf.h")~)+)?(~("[a-zA-Z0-9_/]+\.hpp"~)+)?~+'
#  if ! grep -E "^(#include|$)" "$file" | sed 's/#include //g' | tr '\n' '~' | grep -wE "$REGEX_FRONT"'(~(<[a-z_]+>~)+)?(~(("physfs.h"|"SDL2/SDL.h"|"SDL2/SDL_image.h"|"SDL2/SDL_ttf.h")~)+)?(~("[a-zA-Z0-9_/]+\.hpp"~)+)?~+' &> /dev/null; then
#    echo "Improper header format in $file" >&2
#    exit 1
#  fi
done

echo "[===] Checking #include's: done"
