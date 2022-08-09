#!/bin/bash
fibo() {
if [[ "$1" -eq "0" || "$1" -eq "1" ]]
then
 echo $1
else 
 echo $(( $(fibo $(($1-1)) ) + $(fibo $(($1-2)) ) ))
fi
}

echo "Script started."
echo -n "Insert N fibo number: "
read NUM
RES=$(fibo NUM)
echo "$RES"
