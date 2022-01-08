#!/bin/bash

set -e

if [ -z "$1" ]; then
  echo "Usage: package-release.sh destdir"
  exit 1
fi

NVOPTIX_SRC_DIR=`dirname $(readlink -f $0)`
NVOPTIX_BUILD_DIR=$(realpath "$1")"/nvoptix"

if [ -e "$NVOPTIX_BUILD_DIR" ]; then
  echo "Build directory $NVOPTIX_BUILD_DIR already exists"
  exit 1
fi

function build_arch {
  export WINEARCH="win$1"
  export WINEPREFIX="$NVOPTIX_BUILD_DIR/wine.$1"
  
  cd "$NVOPTIX_SRC_DIR"

  meson --cross-file "$NVOPTIX_SRC_DIR/build-wine$1.txt"  \
        --buildtype "release"                         \
        --prefix "$NVOPTIX_BUILD_DIR/install.$1"         \
        --libdir="lib$1"				\
	--strip                                       \
        "$NVOPTIX_BUILD_DIR/build.$1"

  cd "$NVOPTIX_BUILD_DIR/build.$1"
  ninja install

  mv "$NVOPTIX_BUILD_DIR/install.$1/lib$1" "$NVOPTIX_BUILD_DIR"
  mv "$NVOPTIX_BUILD_DIR/install.$1/fakedlls" "$NVOPTIX_BUILD_DIR/fakedlls$1"
  cp "$NVOPTIX_SRC_DIR/setup_nvoptix.sh" "$NVOPTIX_BUILD_DIR/setup_nvoptix.sh"
  chmod +x "$NVOPTIX_BUILD_DIR/setup_nvoptix.sh"
  rm -R "$NVOPTIX_BUILD_DIR/build.$1"
  rm -R "$NVOPTIX_BUILD_DIR/install.$1"
}

build_arch 64
