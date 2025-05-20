#!/bin/bash

# for each entry in directory tree, except .git, run file
find . -path './git' -prune -o -exec file {} \; | \
  # look for c, awk, and bash files               \
  egrep '(\.([ch]|awk)|Bourne)' |                 \
  # remove ':*' from file command output          \
  sed 's/:.*//g' |                                \
  # pass files to wc -l                           \
  xargs wc -l |                                   \
  # sort on line count                            \
  sort -nk1
