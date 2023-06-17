#!/usr/bin/env bash

set -e

mkdir -p _build
pushd _build
  cmake ..
  cmake --build . --config Debug
popd
