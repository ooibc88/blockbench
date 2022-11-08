#!/bin/bash
cd `dirname ${BASH_SOURCE-$0}`
. env.sh

nohup geth --datadir $ETH_DATA --nodiscover --http --http.addr 0.0.0.0  --http.port "${RPCPORT}" --http.corsdomain "*" --rpc.gascap 0 --maxpeers 32 --networkid 1 --unlock 0 --allow-insecure-unlock --password <(echo -n "${PWD}") --mine --miner.threads 2 > $ETH_DATA/../eth_log 2>&1 &
# 
sleep 1

for com in `cat $ETH_HOME/addPeer.txt`; do
geth --exec $com attach ipc:/$ETH_DATA/geth.ipc
done
