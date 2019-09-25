#!/bin/bash

while true; do
	inotifywait -r ./src -e close_write -e moved_to | while read path action file; do
		echo "$file has changed"

		if [ "${file##*.}" != "c" ] && [ "${file##*.}" != "h" ]; then
			continue;
		fi

		killall gproj

		sleep 1

		make $1 -j8
		if [ $? = 0 ]
		then
			cd build
			./gproj $2 &> last_run_output.txt &
			cd ..
		fi

		sleep 1
	done
done

