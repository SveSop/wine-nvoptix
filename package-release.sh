#!/bin/bash

set -e

if [ -z "$1" ]; then
  echo "Usage: package-release.sh destdir [--fakedlls]"
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
        --prefix "$NVOPTIX_BUILD_DIR/install"             \
        --libdir="x64"                                    \
        --strip                                           \
        "$NVOPTIX_BUILD_DIR/build"

cd "$NVOPTIX_BUILD_DIR/build"
ninja install

mv "$NVOPTIX_BUILD_DIR/install/x64" "$NVOPTIX_BUILD_DIR"

if [ "$2" == "--fakedlls" ]; then
  mv "$NVOPTIX_BUILD_DIR/install/fakedlls" "$NVOPTIX_BUILD_DIR/fakedlls"
else
  rm -R "$NVOPTIX_BUILD_DIR/install/fakedlls"
fi

cp "$NVOPTIX_SRC_DIR/setup_nvoptix.sh" "$NVOPTIX_BUILD_DIR/setup_nvoptix.sh"
chmod +x "$NVOPTIX_BUILD_DIR/setup_nvoptix.sh"
rm -R "$NVOPTIX_BUILD_DIR/build"
rm -R "$NVOPTIX_BUILD_DIR/install"
