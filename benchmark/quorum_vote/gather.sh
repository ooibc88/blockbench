#!/bin/bash
#nnodes
cd `dirname ${BASH_SOURCE-$0}`
. env.sh 

echo "gather.sh"

i=0
for host in `cat $HOSTS`; do
  if [[ $i -lt $1 ]]; then 
    echo "$host"
    echo "admin.addPeer("`ssh  $host $QUO_HOME/enode.sh $host $i 2>/dev/null | grep enode`")" >> addPeer.txt
  fi
  let i=$i+1 
done

cat addPeer.txt
