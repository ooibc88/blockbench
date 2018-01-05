#!/bin/bash

# killall orderer
# killall peer
# nohup ../run-orderer.sh > order.log 2>&1 &
# sleep 4
# nohup ../run-peer.sh > peer.log 2>&1 &
../create_channel.sh
sleep 1
../join_channel.sh
sleep 1
./install_chaincode.sh 
sleep 1 
./instantiate_chaincode.sh 
sleep 3
./invoke_chaincode.sh
