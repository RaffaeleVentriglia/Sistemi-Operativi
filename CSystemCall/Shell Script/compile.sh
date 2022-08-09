#!/bin/bash
#Script to compile raylib files: 
#Usage: 
#

#SCRIPT START
if [ $# -ne 2 ]
 then 
  echo "USAGE: ./scriptname <C file> <Output Name>"
  exit 1
fi

#IFRIGHT
if [ ! -f $1 ]
 then
  echo "File $1 not found, exiting.."
  exit 1
fi

#COMPILE
 gcc $1 -o $2 -I../../src -Iexternal -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
#RUN
 ./$2
exit 0
