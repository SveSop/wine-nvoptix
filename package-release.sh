#!/bin/bash

set -e

if [ $# -lt 2 ]; then
  echo "Usage: $0 releasename destdir [--fakedll]"
  exit 1
fi

VERSION="$1"
DESTDIR="$2"
NVOPTIX_SRC_DIR=$(dirname "$(readlink -f "$0")")
NVOPTIX_BUILD_DIR=$(realpath "$2")"/nvoptix-$VERSION"
shift 2

FAKEDLL=""
LIBDIR='x64'

for arg in "$@"; do
  case "$arg" in
    --fakedll)
      FAKEDLL="-Dfakedll=true"
      LIBDIR='lib'
      ;;
    *)
      echo "Error: unknown option '$arg'"
      exit 1
      ;;
  esac
done

if [ -e "$NVOPTIX_BUILD_DIR" ]; then
  echo "Build directory $NVOPTIX_BUILD_DIR already exists"
  exit 1
fi

cd "$NVOPTIX_SRC_DIR"

meson --cross-file "$NVOPTIX_SRC_DIR/build-wine64.txt"    \
        --buildtype "release"                             \
        --prefix "$NVOPTIX_BUILD_DIR"                     \
        --libdir $LIBDIR                                  \
        --strip                                           \
        $FAKEDLL                                          \
        "$NVOPTIX_BUILD_DIR/build"

cd "$NVOPTIX_BUILD_DIR/build"
ninja install

rm -R "$NVOPTIX_BUILD_DIR/build"

# cleanup
find . -name \*.a -type f -delete
if [ -z "$FAKEDLL" ]; then
  mv "$NVOPTIX_BUILD_DIR/x64/nvoptix.dll.so" "$NVOPTIX_BUILD_DIR/x64/nvoptix.dll"
fi
