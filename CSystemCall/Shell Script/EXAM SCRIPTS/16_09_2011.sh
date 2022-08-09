#!/bin/bash
NOME=""
NOTDONE=true
FOUND=false
echo "Per iniziare, digitare <Comando>."
while [ "$NOTDONE" = "true" ]
do
 read INPUT
 case "$INPUT" in
 "Comando") 
	 echo "Input accettato."
	 NOTDONE=false
	 ;;	
 *) 
	 echo "Per iniziare, digitare <Comando>."
	 ;;
 esac 
done 
#1. creazione file dizionario e inserimento nomi
if [ -e DIZIONARIO ]
then 
 rm DIZIONARIO
 touch DIZIONARIO
else
 touch DIZIONARIO
fi
for x in {0..3}
do 
 echo "Inserisci nome: "
 read NOME
 echo $NOME >> DIZIONARIO
done 
#2. ordine inverso dei nomi
sort -r DIZIONARIO > TMP
cat TMP > DIZIONARIO
rm TMP
#3. ricerca di un nome nel file
echo "Inserisci un nome da ricercare: "
read NOME
for x in $(cat DIZIONARIO)
do
 if [ "$x" = "$NOME" ]
 then
  echo "Il nome $NOME è stato trovato."
  FOUND=true
 fi
done

if [ "$FOUND" = "false" ]
then 
 echo "Il nome $NOME non è stato trovato."
fi
#4. Determinare il size in byte del file:
echo "Il size del file in byte e': $(wc -c DIZIONARIO)"
 
 
 
