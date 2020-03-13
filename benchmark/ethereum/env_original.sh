ETH_HOME=/home/$USER/nfsmount/blockbench/benchmark/ethereum
HOSTS=$ETH_HOME/hosts
RPC_PORT=8545
CLIENTS=$ETH_HOME/clients
ETH_DATA=/home/$USER/eth
LOG_DIR=$ETH_HOME/ycsb_logs
EXE_HOME=$ETH_HOME/../../src/macro/kvstore
#EXE_HOME=/home/$USER/blockbench/src/donothing
BENCHMARK=ycsb
USER=bcuser

##comment these out for smallbank
#EXE_HOME=$ETH_HOME/../../src/smallbank/ethereum/ycsb
#BENCHMARK=smallbank
