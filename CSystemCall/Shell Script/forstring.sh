#!/bin/bash
COUNTER = 0
for STRING in "I'AM" USING THAT FOR ITERATING THROUGH STRINGS
do
	echo $STRING
	((COUNTER++))
done
echo "ITERATED $COUNTER TIMES."
