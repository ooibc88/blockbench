#!/bin/bash
ip_addr=$1
QUORUM=~/download/quorum/build/bin/geth
QUO_HOME=~/blockchain-perf/benchmark/quorum_vote
QUO_DATA=/data/fuwanzeng/quorum_vote
. QUO_HOME/env.sh

${QUORUM} --datadir=$QUO_DATA --rpc --rpcaddr 0.0.0.0 --rpcport "8000" --rpccorsdomain "*" --networkid 9119 --unlock 0 --password <(echo -n "") js <(echo 'console.log(admin.nodeInfo.enode);') 2>/dev/null |grep enode | perl -pe "s/\[\:\:\]/$ip_addr/g" | perl -pe "s/^/\"/; s/\s*$/\"/;"
