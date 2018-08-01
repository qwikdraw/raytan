#!/bin/sh


HOSTS="user@localhost user@localhost user@localhost user@localhost"

I=1
for x in $HOSTS
do
	COMMAND="~/RT/scripts/animation.sh scenes/animation.json -$I.0 0.00833 &"
	echo "Running:" "$COMMAND" "On Machine: $I"
	ssh $x $COMMAND
	I=$((I + 1))
done

TEST=1
while (test $TEST = "1")
do
	echo "Waiting for rendering to finish.."
	sleep 2
	TEST=0
	for x in $HOSTS
	do
		if [ $(ssh $x 'ps -ef | grep RT | wc -l') -gt 0 ]
		then
			TEST=1
		fi
	done
done


for x in $HOSTS
do
	scp -rp $x:~/RT/render/ ./render
done
