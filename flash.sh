#!/bin/bash
if [ "$#" -ne 1 ] || ! [ -d "$1" ]; then
  echo ""
  echo "Argument Missing!!!"
  echo ""
  echo "Usage: $0 Git Clone Parent Directory" >&2
  echo "Ex: $0 $HOME/prime" >&2
  echo ""
  exit 1
fi


CURRENT_USER=$USER

DIR=tq_flash
cd ~/Desktop 

if [[ ! -e $DIR ]]; then
    mkdir $DIR
fi

cd - && cp -r flash.sh ~/Desktop/tq_flash && cd ~/Desktop/
sudo chown -R $CURRENT_USER ~/Desktop/tq_flash
cd ~/Desktop/tq_flash
LOC=$PWD

br_file=border-router.elf
newton_file=newton.elf


reset
echo ""
echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"
echo "+  Please copy TI's .ccxml & .uniflashsession in one common directory +"
echo "+  Directory has been created                                         +"
echo "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++"

echo ""
echo " Source Code Path       : $1  "
echo " Flash Directory Path   : $LOC  "

echo ""

XML=$(ls *.ccxml 2>/dev/null  | wc -l )

if test $XML -le 0; then
    echo " Not Found : .ccxml file, please copy manually"

fi

UFS=$(ls *.uniflashsession 2>/dev/null  | wc -l )
if test $XML -le 0; then
    echo " Not Found : .uniflashsession file, please copy manually"
fi


BR=$(ls $br_file 2>/dev/null  | wc -l )
if test $BR -le 0; then
    echo ""
    echo " Not Found : border-router.elf, Searching the file"
    cp $(find $1 -iname $br_file) .
    echo " $br_file copied to $LOC"
fi

NEWTON=$(ls $newton_file 2>/dev/null  | wc -l )
if test $NEWTON -le 0; then
    echo ""
    echo " Not Found : newton.elf, Searching the file"
    cp $(find $1 -iname $newton_file) .
    echo " $newton_file copied to $LOC"
fi

#find /home/be/work/prime/ -iname newton.elf 

echo ""
echo ""

read -p "Press [Enter] key to continue..." key

reset
cd /opt/ti/
##Flash Border-Router and pause 
flash_br(){
	local m="$@"
	#echo "$m"
        sudo ./uniflash.sh  -ccxml cc2838dk.ccxml -loadSettings cc2838dk.uniflashsession -program "$LOC/border-router.elf"
	read -p "Press [Enter] key to continue..." key
}

##Flash Newton and pause 
flash_newton(){
	local m="$@"
	#echo "$m"
        sudo ./uniflash.sh  -ccxml cc2838dk.ccxml -loadSettings cc2838dk.uniflashsession -program "$LOC/newton.elf"
	read -p "Press [Enter] key to continue..." key
}

while :
do
	reset
        echo ""
	echo "|--------------------------|"
	echo "|   Mote Flashing Script   |"
	echo "|--------------------------|"
	echo "|         M E N U          |"  
	echo "|--------------------------|"
	echo "| 1. Flash Border-Router   |"
	echo "| 2. Flash Newton          |"
	echo "| 3. Exit                  |"
	echo "|--------------------------|"
        echo ""
	read -r -p "Enter option[1-3] : " c

	case $c in
		1) flash_br;;
		2) flash_newton;;
                3) break;;
		*) echo "Select between 1 and 3 only"
	esac
done
