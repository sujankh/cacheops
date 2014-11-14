#!/bin/bash

pth=/sys/devices/system/cpu/cpu0/cache/index
for i in 0 1 2 3 4
do
    if [ -e $pth$i ]
    then
	
	echo -n "Level $(cat $pth$i/level): "
	cat $pth$i/size
    fi
done
