#!/bin/sh

if [ ! -f $1 ]
then
	echo "rt: $1: No such file or directory"
	exit 1
fi

FRAME=0
VALUE=$2
while (test "$FRAME" -le "2400")
do
	NUMBER=$(printf "%04i" "$FRAME")
	echo "Rendering to: $(basename -s '.json' $1)-$NUMBER.png"
	VALUE=$(echo "$VALUE + $3" | bc)
	sed -e "s/\$delta/""$VALUE""/g" $1 | ./RT -d -o "render/$(basename -s '.json' $1)-$NUMBER.png" -
	FRAME=$((FRAME + 1))
done
