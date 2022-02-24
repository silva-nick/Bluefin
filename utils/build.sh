#!/bin/bash

echo "Building.."

set -euo pipefail

mkdir -p /usr/src/bluefin/build && cd build

cmake -DCODE_COVERAGE=ON -DCMAKE_BUILD_TYPE=Debug ..

cmake --build . --config Debug -- -j $(nproc)

ctest -j $(nproc) --output-on-failure
