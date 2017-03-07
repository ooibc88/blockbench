#!/bin/bash
#args: nnodes
cd `dirname ${BASH_SOURCE-$0}`
. env.sh

CONFIG=hl_consensus_$1".yaml"
for peer in `cat $HOSTS`; do
  scp $CONFIG $peer:$HL_SOURCE/consensus/pbft/config.yaml  
done

i=0
for peer in `cat $HOSTS`; do
  if [[ $i -eq 0 ]]; then
    ssh $peer . $HL_HOME/start-root.sh 
    bpeer=$peer
  elif [[ $i -lt $1 ]]; then
    ssh $peer . $HL_HOME/start-slave.sh $bpeer $i
  fi
  let i=$i+1
done
echo bpeer = $bpeer
