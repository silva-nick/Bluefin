#!/bin/bash

echo "Building.."

set -euo pipefail

cd build

cmake --build . --config Debug -- -j $(nproc)

ctest -j $(nproc) --output-on-failure
