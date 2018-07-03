#!/bin/bash
# Args: number of nodes
cd `dirname ${BASH_SOURCE-$0}`
. env.sh

if [ $# -lt 1 ]; then
	NP=`wc -l $HOSTS | cut -d ' ' -f 1`
else
	NP=$1
fi

echo "Starting nodes ($NP) ..."

CONFIG=hl_consensus_$NP".yaml"
for peer in `cat $HOSTS`; do
  scp $CONFIG $peer:$HL_SOURCE/consensus/pbft/config.yaml  
done

i=0
for peer in `cat $HOSTS`; do
  if [[ $i -eq 0 ]]; then
    ssh $peer . $HL_HOME/start-root.sh 
    bpeer=$peer
  elif [[ $i -lt $NP ]]; then
    ssh $peer . $HL_HOME/start-slave.sh $bpeer $i
  fi
  let i=$i+1
done
echo bpeer = $bpeer
