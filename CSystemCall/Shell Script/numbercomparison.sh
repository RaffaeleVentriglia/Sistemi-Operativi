#!/bin/bash
echo "Insert two number to make a comparison:"
read VAR1
read VAR2
#begin the comparison
if [ $VAR1 -gt $VAR2 ]
then 
 echo "$VAR1 is greater than $VAR2"
elif [ $VAR1 -ge $VAR2 ]
then 
 echo "$VAR1 is greater or equal than $VAR2"
elif [ $VAR1 -eq $VAR2 ]
then 
 echo "$VAR1 is equal to $VAR2"
elif [ $VAR1 -le $VAR2 ]
then 
 echo "$VAR1 is lesser or equal to $VAR2"
elif [ $VAR1 -lt $VAR2 ]
then 
 echo "$VAR1 is lesser than $VAR2"
fi 
