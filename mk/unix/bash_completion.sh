#!/usr/bin/env bash

_stmeltdown_completions()
{
  local count="${#COMP_WORDS[@]}"
  local word="${COMP_WORDS[$(($count - 1))]}"

  if [ "$(($count > 2))" = "1" ]; then
    if [ "${COMP_WORDS[$(($count - 2))]}" = "--data" ]; then
      COMPREPLY=($(compgen -d -- "$word"))
      return
    fi
  fi

  # TODO: Make single-letter arguments here work
  STM_ARGS=()
  #STM_ARGS+=('-d')
  STM_ARGS+=('--data')
  #STM_ARGS+=('-h')
  STM_ARGS+=('--help')
  #STM_ARGS+=('-t')
  STM_ARGS+=('--test')
  #STM_ARGS+=('-v')
  STM_ARGS+=('--version')
  COMPREPLY=($(compgen -W "${STM_ARGS[*]}" -- "$word"))
}

complete -o filenames -F _stmeltdown_completions stmeltdown
