#!/bin/bash

set -euo pipefail


echo "Running clang-format on all files.."
files=(
  $(find src include tests -name \*.hpp -print -o -name \*.cpp -print)
)

for f in "${files[@]}"; do
  echo "$f"
  clang-format --verbose -i -style=file "$f" 2>&1 | tr -d "\n"
done
echo
echo "Done formatting"
