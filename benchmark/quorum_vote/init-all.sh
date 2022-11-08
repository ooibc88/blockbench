#!/bin/bash
# num_nodes
cd `dirname ${BASH_SOURCE-$0}`
. env.sh

echo "init-all"
i=0
for host in `cat $HOSTS`; do
  if [[ $i -lt $1 ]]; then
    ssh -oStrictHostKeyChecking=no  $host $QUO_HOME/init.sh $i $1
    echo done node $host
  fi
  let i=$i+1
done
