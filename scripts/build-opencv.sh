#!/bin/sh

PROJECT_ROOT="$(cd "$(dirname "$0")"/.. && pwd)"

echo $PROJECT_ROOT

if [ -d "$PROJECT_ROOT/opencv-build" ]; then
    rm -rf "$PROJECT_ROOT/opencv-build"
fi

mkdir "$PROJECT_ROOT/opencv-build"
cd "$PROJECT_ROOT/opencv-build"

cmake "$PROJECT_ROOT/dependencies/opencv" \
    -DCMAKE_INSTALL_PREFIX="$PROJECT_ROOT/opencv-install" \
    -DBUILD_SHARED_LIBS=ON \
    -DBUILD_opencv_world=ON \
    -DBUILD_TESTS=OFF \
    -DBUILD_PERF_TESTS=OFF \
    -DBUILD_EXAMPLES=OFF \
    -DWITH_GTK=ON \
    -DWITH_V4L=ON \
    -DWITH_FFMPEG=OFF \
    -DBUILD_opencv_dnn=OFF \
    -DWITH_PROTOBUF=OFF \
    -DWITH_CAFFE=OFF

make 
make install

rm -rf "$PROJECT_ROOT/opencv-build"
