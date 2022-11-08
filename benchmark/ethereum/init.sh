#!/bin/bash
#args: number_of_nodes
cd `dirname ${BASH_SOURCE-$0}`
. env.sh

geth --datadir $ETH_DATA init $ETH_HOME/CustomGenesis"_"$1".json"
if [ -z "$2" ]; then
echo "No private key file specified, an account will be generated"
geth --datadir $ETH_DATA --password <(echo -n "${PWD}") account new;
else
echo "importing account using pwd $PWD"
geth account import --datadir $ETH_DATA --password <(echo -n "${PWD}") $2;
fi
