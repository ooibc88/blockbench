#!/bin/bash
cd `dirname ${BASH_SOURCE-$0}`
. env.sh
ip_addr=$1

#echo Running: geth --datadir $ETH_DATA --authrpc.addr 0.0.0.0 --authrpc.port 8051 --http.corsdomain "*" --rpc.gascap 0 --networkid 12345 -nodiscover console --exec "admin.nodeInfo.enode" attach 2>/dev/null |grep enode | perl -pe "s/\[\:\:\]/$ip_addr/g" | perl -pe "s/^/\"/; s/\s*$/\"/;"

geth --datadir $ETH_DATA --authrpc.addr 0.0.0.0 --authrpc.port 8051 --http.corsdomain "*" --rpc.gascap 0 --networkid 12345 -nodiscover console --exec "admin.nodeInfo.enode" attach 2>/dev/null |grep enode | perl -pe "s/\[\:\:\]/$ip_addr/g" | perl -pe "s/^//; s/\s*$//;"

#geth --datadir $ETH_DATA  --authrpc.addr 0.0.0.0 --authrpc.port $RPCPORT --http.corsdomain "*" --rpc.gascap 0 --networkid 15  js <(echo 'console.log(admin.nodeInfo.enode);') 2>/dev/null |grep enode | perl -pe "s/\[\:\:\]/$ip_addr/g" | perl -pe "s/^/\"/; s/\s*$/\"/;"
#geth --datadir $ETH_DATA --authrpc.addr0.0.0.0 --authrpc.port $RPCPORT --http.corsdomain "*" --rpc.gascap 0 --networkid 15 js <(echo 'console.log(admin.nodeInfo.enode);') 2>/dev/null |grep enode | perl -pe "s/\[\:\:\]/$ip_addr/g" | perl -pe "s/^/\"/; s/\s*$/\"/;"
