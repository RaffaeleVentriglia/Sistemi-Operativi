#!/bin/bash
calculate_power() {
RESULT=0
VALUE=1
if [ $# -gt 2 ] 
then
 echo "Too many arguments."
 exit 1
fi
for x in $(seq 1 $2)
do
 ((RESULT=$VALUE*$1))
 VALUE=$RESULT
done
echo "$RESULT"
return 0
}

echo "Script start."
if [ $# -ne 2 ] 
then 
 echo "USAGE: ./scriptname <Value> <Power>."
 exit 1
fi 
echo "Going into the function.."
echo "$1^$2 is: $(calculate_power $1 $2)"
echo "Returned from function, exiting.."
exit 0
