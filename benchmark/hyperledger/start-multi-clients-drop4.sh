#!/bin/bash
#host num_nodes threads
HOSTS=$1

let i=0
#let K=$2/2
let K=4
for client in `cat $HOSTS`; do
  if [[ $i -lt $K ]]; then
    echo $client index $i
    ssh -oStrictHostKeyChecking=no $client 'cd /users/dinhtta/blockchain-perf/hyperledger-fabric/scripts ; ./start-clients-drop4.sh '$3 $i $2
  fi
  let i=$i+1
done

let M=$2*10+320
let SR=$M-250

sleep 250 
let idx=$2-4
let i=0
for server in `cat hosts`; do
  if [[ $i -ge $idx ]]; then
    ssh -oStrictHostKeyChecking=no $server killall -KILL peer java 
    echo "Dropped "$server
  fi
  let i=$i+1
done

sleep $SR
let i=0
for client in `cat $HOSTS`; do
  if [[ $i -lt $K ]]; then
    ssh -oStrictHostKeyChecking=no $client 'killall -KILL ycsbc' 
  fi
  let i=$i+1
done

