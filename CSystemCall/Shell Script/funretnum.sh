#!/bin/bash
return_num() {
RET=$(($1+1))
return $RET #return exit code
#better solution: echo $RET (for number) echo "$RET" for string.
}

echo "Script start."
echo -n "Insert a number: "
read NUMBER
echo "Passing the number to the function.."
return_num $NUMBER
#this is not a proper mode to pass returning value, because we're not passing the "real"
#value, but just it's exit code, that's valid because exit codes got a range from 0 to 255.
#the best mode to pass something and return it, it's to catch it with a "echo".
#ex: RETNUM=$(return_num $NUMBER) 
RETNUM=$?
echo "Number returned is: $RETNUM"



