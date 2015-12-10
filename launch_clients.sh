#!/bin/bash
#SERV_IP=0x867E8DDE

HOSTS="l25008 l25009 l25010 l25011 l25012"
USERNAME=$1
SCRIPT="cd ~/FA15/CS361/phase4/p4; ./client"

if [ -n "$USERNAME" ]
then
    for HOSTNAME in ${HOSTS} ; do
	echo "Launching client on ${HOSTNAME}.."
	gnome-terminal -e "ssh ${USERNAME}@${HOSTNAME} ${SCRIPT}"	
	#ssh -f ${USERNAME}@${HOSTNAME} "${SCRIPT}"
    done
else 
    echo "username required"
fi

