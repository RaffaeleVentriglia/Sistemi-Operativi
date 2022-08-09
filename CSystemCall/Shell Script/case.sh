#!/bin/bash
read FRUIT;
case "$FRUIT" in
"apple"|"Apple"|"APPLE") 
	echo "I love apples!"
	;;
"orange"|"Orange"|"ORANGE")
	echo "I love oranges!"
	;;
Z*|z*)
	echo "everything with Z is accepted."
	;;
*Z|*z)
	echo "Everything that finish with z is accepted."
	;;
*)
	echo "Dude ur gay?"
	;;
esac
