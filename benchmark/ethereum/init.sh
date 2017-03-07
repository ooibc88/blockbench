#!/bin/bash
#args: number_of_nodes
cd `dirname ${BASH_SOURCE-$0}`
. env.sh

geth --datadir=$ETH_DATA init $ETH_HOME/CustomGenesis"_"$1".json"
geth --datadir=$ETH_DATA --password <(echo -n "") account new
