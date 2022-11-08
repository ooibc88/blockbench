# Environment variables for Hyperledger in Blockbench
# 
# If not set, HL_HOME is the folder of this script. Please uncomment 
# and change according to your setup.
# HL_HOME=
if [ -z "$HL_HOME" ]; then
	cd `dirname ${BASH_SOURCE-$0}`
	HL_HOME=`pwd`
fi

# peer hosts
HOSTS=$HL_HOME/hosts

# driver hosts
CLIENTS=$HL_HOME/clients

# HL_DATA can be shared among hosts. By default, we set it to ./data
HL_DATA=$HL_HOME/data

# Hyperledger fabric source folder (install.sh puts go in $HL_DATA)
HL_SOURCE=$HL_DATA/go/src/github.com/hyperledger/fabric

# Log dir
LOG_DIR=$HL_HOME/logs

# Define benchmark, which can be: donothing, ycsb or smallbank
BENCHMARK=donothing
# BENCHMARK=ycsb
# BENCHMARK=smallbank

# For ycsb and donothing, the driver is in src/macro/kvstore
EXE_HOME=$HL_HOME/../../src/macro/kvstore

# For smallbank, the driver is in src/macro/smallbank (uncomment)
# EXE_HOME=$HL_HOME/../../src/macro/smallbank
