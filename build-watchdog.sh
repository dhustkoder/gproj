#!/bin/bash

gproj_pid=false

while true; do
	inotifywait -r ./src/* --exclude game --exclude sdl2 -e close_write

	if [ $gproj_pid != false ]
	then
		kill $gproj_pid
	fi

	sleep 1

	make $1 -j8
	if [ $? = 0 ]
	then
		cd build
		./gproj $2 &
		gproj_pid=$!
		cd ..
	fi

	sleep 1
done

