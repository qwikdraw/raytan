#!/bin/sh

if [ ! -f $1 ]
then
	echo "rt: $1: No such file or directory"
	exit 1
fi

old_checksum=""
while test -e $1
do
	checksum=$(md5 $1)
	if [ "$checksum" != "$old_checksum" ]
	then
		echo "Rendering scene: $1"
		./RT -d $1
		printf "Done.\n\n"
	fi
	old_checksum=$checksum
	sleep 1
done
