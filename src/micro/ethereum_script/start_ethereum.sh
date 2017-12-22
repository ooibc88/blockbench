#!/bin/bash
ETH_DATA=`dirname ${BASH_SOURCE-$0}`/../data
cd `dirname ${BASH_SOURCE-$0}`
pwd 

killall geth

rm -rf $ETH_DATA/{geth,keystore}
mkdir -p $ETH_DATA/keystore
cp ./key/* $ETH_DATA/keystore

# ------------private chain(10 accounts)------------
geth --datadir=$ETH_DATA init genesis.json 
geth --datadir=$ETH_DATA --nodiscover --rpcapi="db,eth,net,web3,personal,web3" --rpc --rpcaddr "localhost" --rpcport "8545" --rpccorsdomain "*" --gasprice 0 --maxpeers 32 --networkid 9119 --unlock "0x12f029d57082315085bfb4d4d8345c92c5cdd881" --password <(echo -n "") --mine --minerthreads 4
