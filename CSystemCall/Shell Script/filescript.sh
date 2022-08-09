#!/bin/bash
echo "Insert filename:"
read FILENAME
if [ -e "$FILENAME" ]
then 
 echo "File already existent."
 echo "Would you like to delete it and create a new one (y/n)"
 read ANSWER
 if [ "$ANSWER" = "y" ]
 then 
  rm $FILENAME
  touch $FILENAME
  chmod +x $FILENAME
  echo "CREATED" > $FILENAME
  echo "File created and added execution rights."  
 else 
  echo "Exiting."
  exit 1
 fi
else  
 touch $FILENAME
 chmod +x $FILENAME
 echo "CREATED" > $FILENAME
 echo "File created and added execution rights."
fi
