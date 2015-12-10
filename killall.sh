HOSTS="l25008 l25009 l25010 l25011 l25012"
USERNAME=$1
SCRIPT="killall -9 client"
for HOSTNAME in ${HOSTS} ; do
        #gnome-terminal -e "ssh -X $1@${CLIENTS[i]}" &       
        echo "${HOSTNAME}:"
	ssh -X $1@${HOSTNAME} "${SCRIPT}"
done
