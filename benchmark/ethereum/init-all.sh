#!/bin/bash
# num_nodes
cd `dirname ${BASH_SOURCE-$0}`
. env.sh

i=0
for host in `cat $HOSTS`; do
  if [[ $i -lt $1 ]]; then
    #$1 is the number of nodes
    #$2 is the private key file to import for use in geth
    ssh -oStrictHostKeyChecking=no $USER@$host $ETH_HOME/init.sh $1 $2
    echo done node $host
  fi
  let i=$i+1
done
