#!/bin/sh
make $@
if [ "$?" != 0 ]; then
	exit
fi
ln -s $(pwd)/assets/* $(pwd)/build 2>&-
cd build
./gproj
cd ..
