#!/bin/bash

echo -n "Inserisci il nome del file su cui operare: "
read FILENAME

if [ -e $FILENAME ] 
then
 echo "File trovato."
else 
 echo "File non trovato, esco."
 exit 1
fi

#1)counting lines
echo "Numero di linee: $(wc -l out.txt)"

#2)printing only Name and city 

#3) changing "COmmerciante" with "nulla"
sed 's/ Commerciante/ Nulla/' out.txt
