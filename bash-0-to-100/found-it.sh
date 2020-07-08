#!/bin/bash

function main {
  if grep -q $1 $2; then #<1><2><3>
    echo "found it!"
  fi
}

main $@
