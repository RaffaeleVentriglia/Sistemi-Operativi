#!/bin/bash
return_fun_par() {
if [ "$#" -ne "2" ]
then 
 echo "Exceeded parameters."
 exit 1
else
 NUMPAR=$#
 echo $NUMPAR
fi
}
#begin
echo "Script started."
echo "Calling function, and passing 3 parameters to it."
RETPAR=$(return_fun_par 1 2 3)
echo "Returned. Parameter passed: $RETPAR"
echo "Calling function, and passing 2 parameters to it."
RETPAR=$(return_fun_par 1 2)
echo "Returned. Parameter passed: $RETPAR"
echo "Exiting.."
exit 0
