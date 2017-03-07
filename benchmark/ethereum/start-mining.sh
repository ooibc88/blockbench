#!/bin/bash
cd `dirname ${BASH_SOURCE-$0}`
. env.sh

nohup geth --datadir=$ETH_DATA --nodiscover --rpc --rpcaddr 0.0.0.0 --rpcport "8000" --rpccorsdomain "*" --gasprice 0 --maxpeers 32 --networkid 9119 --unlock 0 --password <(echo -n "") --mine --minerthreads 8 > $ETH_DATA/../eth_log 2>&1 &

sleep 1

for com in `cat $ETH_HOME/addPeer.txt`; do
  geth --exec $com attach ipc:/$ETH_DATA/geth.ipc
done
