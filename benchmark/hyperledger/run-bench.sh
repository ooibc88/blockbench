#!/bin/bash
#arg num_nodes #num_threads num_clients txrate [-drop]

cd `dirname ${BASH_SOURCE-$0}`
. env.sh

./stop-peers.sh
sleep 2
./start-peers.sh $1
sleep 5

./start-multi-clients.sh $3 $1 $2 $4 $5 &
BACK=$!
#sleep 100
#python partition.py $1
wait $BACK
./stop-peers.sh
sleep 5
