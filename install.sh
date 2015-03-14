#!/bin/bash

START=$(date +%s)

function countdown(){
   date1=$((`date +%s` + 60)); 
   while [ "$date1" -ne `date +%s` ]; do 
     echo -ne "Time Left : $(date -u --date @$(($date1 - `date +%s`)) +%S)\r";
     sleep 0.1
   done
}

reset
printf "\e[1;35m**********************************\e[0m\n"
printf "\e[1;35m**      Contiki Installer       **\e[0m\n"
printf "\e[1;35m**********************************\e[0m\n"

key=0
tabs 4
bold=`tput bold`
normal=`tput sgr0`
SLEEP=1

DIR=$HOME/contiki_test
C_DIR=$DIR/contiki
LOG_FILE='/var/log/Contiki_Installer.log'

sleep $SLEEP

printf "\e[1;33m** Checking user privilage...\e[0m\n"
if [[ $EUID -ne 0 ]]; then
   echo " "
   printf "\t[\e[1;31mFAIL\e[0m]\n"
   echo " "
   echo -e "\t$(tput setaf 1)[ERROR] ${bold}This script must be run with $(tput setab 7)sudo privilage${normal}$(tput sgr 0)"
   echo " "
   exit 1
else
   echo " "
   printf "\t[\e[1;32mPASS\e[0m]\n"
   echo " "
fi

rm -rf $LOG_FILE
sleep $SLEEP

printf "\e[1;33m** Checking for Internet connectivity...\e[0m\n"
if [[ "$(nm-online -t 1)" != "" ]]; then
   printf "\t[\e[1;31mFAIL\e[0m]\n"
   echo -e "$(tput setaf 1)[ERROR] \t${bold}Check Wifi or Network cable, $(tput setab 7)unable to reach gateway${normal}...$(tput sgr 0)"
   echo " "
   exit 1
else
   echo " "
   printf "\t[\e[1;32mPASS\e[0m]\n"
fi

if [[ "$(ping -c 1 8.8.8.8 | grep '100% packet loss' )" != "" ]]; then
   echo "No Internet Connection..."
   echo " "
   exit 1
fi

sleep $SLEEP

echo " "
printf "\e[1;33m** Checking for system updates...\e[0m\n"

sleep $SLEEP

apt-get update
echo " "
printf "\n\n\t[\e[1;32mDONE\e[0m]\n"

echo " "
printf "\e[1;33m** Downloading & Installing dependency file...\e[0m\n"
echo " "
apt-get install -y gcc-msp430 git openjdk-7-jdk ant vim terminator python-pip python-dev
echo " "
printf "\n\n\t[\e[1;32mDONE\e[0m]\n"

echo " "
printf "\e[1;33m** Installing txthings...\e[0m\n"

echo " "
pip -q install txthings
echo " "
printf "\n\n\t[\e[1;32mDONE\e[0m]\n"

echo " "
printf "\e[1;33m** Setting JAVA_HOME environment variable...\e[0m\n"
export JAVA_HOME="/usr/lib/jvm/java-7-openjdk/"
printf "\e[1;32m\tJAVA_HOME : $JAVA_HOME\e[0m\n"

echo " "
printf "\e[1;33m** Creating Directory...\e[0m\n"

if [[ ! -e $DIR ]]; then
    mkdir $DIR
    cd $DIR
    echo " "
    printf "\e[1;33m** Cloning Contiki from GitHub...\e[0m\n"
    if [[ ! -e $C_DIR ]]; then
        echo " "
    	git clone https://github.com/ManikantThakur/contiki.git
    	echo -e "** Cloning...\c"
    	printf "[\e[1;32mPASS\e[0m]\n"
        
        echo " "
        cd $C_DIR
        printf "\e[1;33m** Updating Contiki submodules [cc2538-bsl and mspsim]...\e[0m\n"
        git submodule update --init

        echo " "
        printf "\e[1;33m** Updating submodules...\e[0m\n"
#        echo -e "** Updating submodules...\c"
        printf "[\e[1;32mPASS\e[0m]\n"
    else
        echo " "
        printf "\e[1;32m\tDirectory : [ contiki ] already exists...\e[0m\n"

        pwd
        echo $DIR
        cd $DIR
        echo ""
        echo ""
        printf "\e[1;33m**  Updating repo...\e[0m\n"
        git pull        
    fi
else
    echo " "
    printf "\e[1;32m\tDirectory : [ $DIR ] already exists...\e[0m\n"

    echo ""
    printf "\e[1;31m\tIf cloning was not sucessfull, delete $DIR and re-run this script...\e[0m\n"
fi

echo ""
echo ""
echo -e "** \e[1;32mFor further manual steps, please access the file at '$LOG_FILE', after system reboots\e[0m\n"
echo -e "** After reboot, set the JAVA_HOME to "/usr/lib/jvm/java-7-openjdk/""
echo -e "** Navigate to path : $HOME/contiki_test/contiki/examples/newton/"
echo -e "** Execute the command : make TARGET=cooja newton.csc"
echo -e "** Open new terminal, perform the below steps"
echo -e "** Navigate to path : $HOME/contiki_test/contiki/examples/ipv6/rpl-border-router/"
echo -e "** Execute the command 'make connect-router-cooja'"
echo ""

if [[ $1 = "-r" ]]; then 
    echo -e "** After reboot, set the JAVA_HOME to "/usr/lib/jvm/java-7-openjdk/""              &>> $LOG_FILE
    echo -e "** Navigate to path : $HOME/contiki_test/contiki/examples/newton/"                 &>> $LOG_FILE
    echo -e "** Execute the command : make TARGET=cooja newton.csc"                             &>> $LOG_FILE
    echo -e "** Open new terminal, perform the below steps"                                     &>> $LOG_FILE
    echo -e "** Navigate to path : $HOME/contiki_test/contiki/examples/ipv6/rpl-border-router/" &>> $LOG_FILE
    echo -e "** Execute the command 'make connect-router-cooja'"                                &>> $LOG_FILE
    printf "\e[1;31m*********************************\e[0m\n"
    printf "\e[1;31m** System will be rebooted...  **\e[0m\n"
    printf "\e[1;31m*********************************\e[0m\n"
    printf "\e[1;31m** Rebooting system in 60 seconds, save you work...\e[0m\n"
    countdown 
    reboot
fi

END=$(date +%s)
DIFF=$(( $END - $START ))

echo ""
echo ""
printf "\e[1;32m\tTime Taken : $DIFF Seconds.\e[0m\n"
echo ""
echo ""
