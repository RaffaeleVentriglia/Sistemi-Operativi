#!/bin/bash
echo "Insert a value (1 reccomended):"
read VALUE
EMPTY=1
#here we use test instead of [] to test if the string is empty
if test -z $VALUE 
then
 echo "You inserted an empty string."
#here we use [] to demonstrate the test and [] are equal. 
#NB: to check the condition, everything must be separed with s
elif [ "$EMPTY" = "$VALUE" ]
then
 echo "Nice, you heard my reccomendations."
else 
 echo "You didn't hear my reccomendations."
fi


