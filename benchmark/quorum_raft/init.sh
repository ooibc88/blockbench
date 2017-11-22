#!/bin/bash
#args: number_of_nodes
cd `dirname ${BASH_SOURCE-$0}`
. env.sh

echo "init.sh"
mkdir -p $QUO_DATA/{keystore,geth}

let i=$1+1
echo "keys/key$i"
cp keys/key$i $QUO_DATA/keystore
cp raft/static-nodes$2.json $QUO_DATA/static-nodes.json
cp raft/nodekey$i $QUO_DATA/geth/nodekey
${QUORUM} --datadir=$QUO_DATA init $QUO_HOME/genesis_quorum.json
