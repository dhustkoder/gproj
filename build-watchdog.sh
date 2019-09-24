#!/bin/bash

gproj_pid=false

while true; do
	inotifywait -r ./src -e close_write

	if [ $gproj_pid != false ]
	then
		kill $gproj_pid
	fi

	sleep 1

	make $1 -j8

	cd build
	./gproj $2 &
	gproj_pid=$!
	cd ..
	
	sleep 1
done

