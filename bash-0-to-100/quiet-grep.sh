#!/bin/bash

function quiet_grep {
  if grep -q $1 $2; then
    return 0 #<1>
  else
    return 1
  fi
}

function main {
  if quiet_grep $1 $2; then #<2>
    echo "found it!"
  fi
}

main $@
