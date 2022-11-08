#!/bin/bash
#args: number_of_nodes
cd `dirname ${BASH_SOURCE-$0}`
. env.sh

echo "init.sh"
mkdir -p $QUO_DATA/{keystore,geth}

let i=$1+1
echo "$i..$1...$2"

cp static-nodes/static-nodes$2.json $QUO_DATA/static-nodes.json
cp nodekey/nodekey$2/$1/nodekey $QUO_DATA/geth/nodekey
#cat static-nodes/static-nodes$2.json
#echo static-nodes/static-nodes$2.json
#echo nodekey/nodekey$2/$1/nodekey

${QUORUM} --datadir=$QUO_DATA init $QUO_HOME/genesis$2.json
${QUORUM} --datadir=$QUO_DATA account new --password <(echo -n "")
