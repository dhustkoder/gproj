#!/bin/sh
make $@
ln -s $(pwd)/assets/* $(pwd)/build 2>&-
cd build
./gproj
cd ..
