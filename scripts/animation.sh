#!/bin/sh

if [ ! -f $1 ]
then
	echo "rt: $1: No such file or directory"
	exit 1
fi

FRAME=0
VALUE=$2
while (test "$FRAME" -le "150")
do
	NUMBER=$(printf "%04i" "$FRAME")
	echo "Rendering to: $(basename -s '.json' $1)-$NUMBER.png"
	VALUE=$(echo "$VALUE + $3" | bc)
	sed -e "s/\$delta/""$VALUE""/g" $1 | ./RT -d -o "render/$(basename -s '.json' $1)-$NUMBER.png" -
	FRAME=$((FRAME + 1))
done

#ffmpeg -framerate 25 -f image2 -i ./scene-%04d.png -c:v libvpx-vp9 -pix_fmt yuva420p -pass 1 -b:v 0 -crf 33 -threads 4 -speed 4 -tile-columns 6 -frame-parallel 1 -an -f webm /dev/null

#ffmpeg -framerate 25 -f image2 -i ./scene-%04d.png -c:v libvpx-vp9 -pix_fmt yuva420p -vf scale=1024:-1 -pass 2 -b:v 0 -crf 18 -threads 4 -speed 2 -tile-columns 6 -frame-parallel 1 -auto-alt-ref 1 -lag-in-frames 25 -b:a 64k -f webm out.webm
