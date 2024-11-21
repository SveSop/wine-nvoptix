#!/bin/bash

set -e

if [ -z "$1" ]; then
  echo "Usage: package-release.sh destdir"
  exit 1
fi

NVOPTIX_SRC_DIR=$(dirname "$(readlink -f "$0")")
NVOPTIX_BUILD_DIR=$(realpath "$1")"/nvoptix"

if [ -e "$NVOPTIX_BUILD_DIR" ]; then
  echo "Build directory $NVOPTIX_BUILD_DIR already exists"
  exit 1
fi

cd "$NVOPTIX_SRC_DIR"

meson --cross-file "$NVOPTIX_SRC_DIR/build-wine64.txt"    \
        --buildtype "release"                             \
        --prefix "$NVOPTIX_BUILD_DIR"                     \
        --libdir="x64"                                    \
        --strip                                           \
        "$NVOPTIX_BUILD_DIR/build"

cd "$NVOPTIX_BUILD_DIR/build"
ninja install

mv "$NVOPTIX_BUILD_DIR/x64/nvoptix.dll.so" "$NVOPTIX_BUILD_DIR/x64/nvoptix.dll"
rm -R "$NVOPTIX_BUILD_DIR/build"
