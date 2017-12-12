#!/bin/sh
mkdir -p build
cmake -B./build/ -H./
make -C ./build -j4
