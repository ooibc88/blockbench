#!/bin/bash
#arg: master ID

cd `dirname ${BASH_SOURCE-$0}`
. env.sh

PEER="$HL_SOURCE/build/bin/peer"
if ! [ -e $PEER ]; then
	echo "Hyperledger peer executable not found: $PEER"
	exit 1
fi

export CORE_PEER_ID=vp$2
export CORE_PEER_DISCOVERY_ROOTNODE=$1:7051
export CORE_PEER_ADDRESSAUTODETECT=true
export CORE_PEER_NETWORKID=dev2
export CORE_PEER_VALIDATOR_CONSENSUS_PLUGIN=pbft
export CORE_PEER_FILE_SYSTEM_PATH=$HL_DATA/hyperledger
export CORE_VM_ENDPOINT=http://0.0.0.0:2375
export CORE_PBFT_GENERAL_MODE=batch

rm -rf $CORE_PEER_FILE_SYSTEM_PATH
mkdir -p $CORE_PEER_FILE_SYSTEM_PATH
cd $HL_SOURCE/build/bin
export LD_LIBRARY_PATH=/usr/local/lib
export GOPATH=$HL_DATA/go
HOST=`hostname`
nohup $PEER node start > $HL_DATA/hl_log_slave_$HOST 2>&1 &
