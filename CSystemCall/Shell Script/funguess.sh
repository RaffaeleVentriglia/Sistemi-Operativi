#!/bin/bash
fun_guess() {
if [ $1 -ne 3 ] 
then 
 echo "Wrong Number. Better luck next time!"
 exit 1
else 
 echo "You got it!"
fi
}

echo "Script started."
echo -n "Guess the number: "
read NUM
RES_STRING=$(fun_guess $NUM)
echo "Returned from function, resultant string is:"
echo "$RES_STRING"
exit 0
