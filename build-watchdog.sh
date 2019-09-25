#!/bin/bash


WATCH_COMMAND=""
READ_COMMAND=""


if [ $OSTYPE = "linux-gnu" ]; then
	WATCH_COMMAND="inotifywait -r ./src -e close_write -e moved_to"
	READ_COMMAND="read path action file"
else
	WATCH_COMMAND="watchman-wait ./src -m 0"
	READ_COMMAND="read file"
fi


while true; do
	$WATCH_COMMAND | while $READ_COMMAND; do
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


