#!/bin/bash
cd `dirname ${BASH_SOURCE-$0}`
. env.sh
echo "start-mining.sh"

if [[ $1 -eq 0 ]]; then
    echo $1
    nohup ${QUORUM} --datadir $QUO_DATA --rpccorsdomain "*" --nodiscover --syncmode full --mine --rpc --rpcaddr 0.0.0.0 --rpcapi admin,db,eth,debug,miner,net,shh,txpool,personal,web3,quorum,istanbul --rpcport 8000 --unlock 0 --password <(echo -n "") > $QUO_DATA/../pbft_quorum_log 2>&1 &
else
    nohup ${QUORUM} --datadir $QUO_DATA --rpccorsdomain "*" --nodiscover --syncmode full --mine --rpc --rpcaddr 0.0.0.0 --rpcapi admin,db,eth,debug,miner,net,shh,txpool,personal,web3,quorum,istanbul --rpcport 8000 --unlock 0 --password <(echo -n "") > $QUO_DATA/../pbft_quorum_log 2>&1 &
fi
