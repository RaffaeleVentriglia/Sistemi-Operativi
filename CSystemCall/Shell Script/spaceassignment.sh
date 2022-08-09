#!/bin/bash
PWD=`pwd`
echo "Current working directory is: $PWD"
#PWD= something means that PWD= is a "temporary" empty variables that we assign to the ls command
#is useful to call a command and modify a variable only in that line
PWD= ls
echo $PWD
