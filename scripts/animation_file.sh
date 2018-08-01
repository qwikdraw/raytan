#!/bin/sh

if [ ! -f $1 ]
then
	echo "rt: $1: No such file or directory"
	exit 1
fi

trap catch 1 2 3 6

catch()
{
	exit 1
}

mkdir -p render
ANIMATION=$(cat $2)
TOTAL_FRAMES=$(printf "$ANIMATION" | head -n 1)
ANIMATION=$(printf "$ANIMATION" | tail -n +2)
FRAME=0
while (test "$FRAME" -le "$TOTAL_FRAMES")
do
	FILE_NUMBER=$(printf "%04i" "$FRAME")
	echo "Rendering: $FILE_NUMBER.png"
	SCENE=$(cat $1)
	for RULE in $ANIMATION
	do
		RULE=$(printf "$RULE" | tr '|' '\t')
		NAME=$(printf "$RULE" | cut -f 1)
		START=$(printf "$RULE" | cut -f 2)
		DELTA=$(printf "$RULE" | cut -f 3)
		VALUE=$(printf "$START + ($DELTA * $FRAME)\n" | bc | sed "s/^\./0./g")
		SCENE=$(printf "$SCENE" | sed "s/$NAME/""$VALUE""/g")
	done
	printf "$SCENE" | ./RT -d -o "render/$FILE_NUMBER.png" -
	FRAME=$((FRAME + 1))
done

#ffmpeg -framerate 25 -f image2 -i ./scene-%04d.png -c:v libvpx-vp9 -pix_fmt yuva420p -pass 1 -b:v 0 -crf 33 -threads 4 -speed 4 -tile-columns 6 -frame-parallel 1 -an -f webm /dev/null

#ffmpeg -framerate 25 -f image2 -i ./scene-%04d.png -c:v libvpx-vp9 -pix_fmt yuva420p -vf scale=1024:-1 -pass 2 -b:v 0 -crf 18 -threads 4 -speed 2 -tile-columns 6 -frame-parallel 1 -auto-alt-ref 1 -lag-in-frames 25 -b:a 64k -f webm out.webm
