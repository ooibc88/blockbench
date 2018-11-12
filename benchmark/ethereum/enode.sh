#!/bin/bash
cd `dirname ${BASH_SOURCE-$0}`
. env.sh
ip_addr=$1

echo Running: geth --datadir=$ETH_DATA --rpc --rpcaddr 0.0.0.0 --rpcport $RPCPORT --rpccorsdomain "*" --gasprice 0 --networkid 9119 --unlock 0 --password <(echo -n "${PWD}") js <(echo 'console.log(admin.nodeInfo.enode);') 2>/dev/null |grep enode | perl -pe "s/\[\:\:\]/$ip_addr/g" | perl -pe "s/^/\"/; s/\s*$/\"/;"
geth --datadir=$ETH_DATA --rpc --rpcaddr 0.0.0.0 --rpcport $RPCPORT --rpccorsdomain "*" --gasprice 0 --networkid 9119 --unlock 0 --password <(echo -n "${PWD}") js <(echo 'console.log(admin.nodeInfo.enode);') 2>/dev/null |grep enode | perl -pe "s/\[\:\:\]/$ip_addr/g" | perl -pe "s/^/\"/; s/\s*$/\"/;"
