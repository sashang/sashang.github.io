#!/bin/bash

function main {
  echo "$1 $2" #<1>
}

main $@ #<2>
