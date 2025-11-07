#!/bin/sh

set -e

read -p "This will remove build, opencv-build and directories. You will have to recompile opencv again. Proceed? [y/N]" REPLY
case "$REPLY" in 
    y|Y ) echo "Removing...";;
    * ) exit 0;;
esac

PROJECT_ROOT="$(cd "$(dirname "$0")"/.. && pwd)"

rm -rf "$PROJECT_ROOT/build"
rm -rf "$PROJECT_ROOT/opencv-build"
rm -rf "$PROJECT_ROOT/opencv-install"
