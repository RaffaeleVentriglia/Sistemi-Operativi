#!/bin/bash
echo "Insert a number between 0 and 10."
read NUMBER
while [ $NUMBER -le 10 ]
do 
 echo "Number = $NUMBER"
 NUMBER=$(($NUMBER+1))
 #VARIANTS:
 #NUMBER=$((NUMBER+1))
 #((NUMBER+=1))
 #((NUMBER=NUMBER+1))
 #((NUMBER++))
done
