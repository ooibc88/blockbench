#!/bin/bash

echo "partition.sh"

nodeX=$1
nodeY=$2
time=$3
  
argErr="Args Error: Usage ./killconn <Node 1 ip/name> <Node 2 ip/name> <Time(second)>"

if (($#!=3)); then
echo $argErr
exit 
fi

echo NodeX: $nodeX
echo NodeY: $nodeY
echo Drop Time: $time  

ssh $nodeX sudo iptables -w -A INPUT -p tcp -s $nodeY -j DROP
ssh $nodeY sudo iptables -w -A INPUT -p tcp -s $nodeX -j DROP 
sleep $time
ssh $nodeX sudo iptables -w -D INPUT -p tcp -s $nodeY -j DROP
ssh $nodeY sudo iptables -w -D INPUT -p tcp -s $nodeX -j DROP


















