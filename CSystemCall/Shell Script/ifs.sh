#!/bin/bash
var=HELLO 
echo "$var"
echo "SHELL PID: $$"
echo "Actual program in esecution: $0"
echo "First argument passed: $1"
echo "Second argument passed: $2"
echo "Third argument passed $3"
echo "List of all argument passed with cash star: $*"
echo "List of all argument passed with cash snail: $@"
echo "Resetting IFS to ''"
IFS=''
echo "List of all argument passed with cash star $*"
echo "List of all argument passed with cash snail $@"
