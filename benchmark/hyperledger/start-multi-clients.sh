#!/bin/bash
#num_clients num_nodes threads txrate [-drop]
cd `dirname ${BASH_SOURCE-$0}`
. env.sh

let i=0
let K=$1/2
for client in `cat $CLIENTS`; do
  if [[ $i -lt $K ]]; then
    echo $client index $i
    ssh -oStrictHostKeyChecking=no $client $HL_HOME/start-clients.sh $3 $i $2 $4
  fi
  let i=$i+1
done

if [[ $5 == '-drop' ]]; then
  let M=$2*10+320
  let SR=$M-250
  sleep 250 
  let idx=$2-4
  let i=0
  for server in `cat $HOSTS`; do
    if [[ $i -ge $idx ]]; then
      ssh -oStrictHostKeyChecking=no $server killall -KILL peer java 
      echo "Dropped "$server
    fi
    let i=$i+1
  done
  sleep $SR
  let i=0
  for client in `cat $CLIENTS`; do
    if [[ $i -lt $K ]]; then
      ssh -oStrictHostKeyChecking=no $client 'killall -KILL driver' 
    fi
    let i=$i+1
  done
else
  let M=$2*10+300
  sleep $M
  let i=0
  for client in `cat $CLIENTS`; do
    if [[ $i -lt $K ]]; then
      ssh -oStrictHostKeyChecking=no $client 'killall -KILL driver' 
    fi
    let i=$i+1
  done
fi

