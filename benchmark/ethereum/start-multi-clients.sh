#!/bin/bash
#num_clients num_nodes threads tx_rate [-drop]
cd `dirname ${BASH_SOURCE-$0}`
. env.sh

let i=0
let IDX=$1/2
for client in `cat $CLIENTS`; do
  if [[ $i -lt $IDX ]]; then
    echo $client index $i
#  ssh -oStrictHostKeyChecking=no $client 'cd /users/dinhtta/blockchain-perf/ethereum ; ./start-clients.sh '$3 $i $2
    ssh -oStrictHostKeyChecking=no $client $ETH_HOME/start-clients.sh $3 $i $2 $4
  fi
  let i=$i+1
done

if [[ $5 == "-drop" ]]; then
  let M=$2*10+240
  let SR=$M-150
  sleep 250 
  let idx=$2-4
  let i=0
  for server in `cat $HOSTS`; do
    if [[ $i -ge $idx ]]; then
      ssh -oStrictHostKeyChecking=no $server killall -KILL geth peer java 
      echo "Dropped "$server
    fi
    let i=$i+1
  done
  sleep $SR
  for client in `cat $CLIENTS`; do
    echo $client index $i
    ssh -oStrictHostKeyChecking=no $client 'killall -KILL driver' 
    let i=$i+1
  done
else
  let M=$2*10+240
  sleep $M
  for client in `cat $CLIENTS`; do
    echo $client index $i
    ssh -oStrictHostKeyChecking=no $client 'killall -KILL driver' 
    let i=$i+1
  done
fi
