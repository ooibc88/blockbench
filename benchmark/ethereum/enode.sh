#!/bin/bash
ip_addr=$1
ETH_DATA=$2
rpc_port=$3
    
geth --datadir=$ETH_DATA --rpc --rpcaddr 0.0.0.0 --rpcport $rpc_port --rpccorsdomain "*" --gasprice 0 --networkid 9119 --unlock 0 --password <(echo -n "") js <(echo 'console.log(admin.nodeInfo.enode);') 2>/dev/null |grep enode | perl -pe "s/\[\:\:\]/$ip_addr/g" | perl -pe "s/^/\"/; s/\s*$/\"/;"

