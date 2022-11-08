#!/bin/bash
# Args: num_nodes num_threads num_clients txrate [-drop]
if [ $# -lt 4 ]; then
	echo "Usage: $0 <num_nodes> <num_threads> <num_clients> <txrate> [-drop]"
	exit 1
fi
NNODES=$1
NCLIENTS=$3
NTHREADS=$2
TXRATE=$4
DROP=$5

cd `dirname ${BASH_SOURCE-$0}`
. env.sh

./stop-peers.sh
sleep 2
./start-peers.sh $NNODES
sleep 5

./start-multi-clients.sh $NCLIENTS $NNODES $NTHREADS $TXRATE $DROP &
BACK=$!
sleep 100
python partition.py $NNODES
wait $BACK
./stop-peers.sh
sleep 5
