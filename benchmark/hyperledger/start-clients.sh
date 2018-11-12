#!/bin/bash
# args=THREADS index N txrate
if [ $# -lt 4 ]; then
	echo "Usage: $0 <threads> <index> <servers> <txrate>"
	exit 1
fi
NTHREADS=$1
INDEX=$2
NSERVERS=$3
TXRATE=$4

cd `dirname ${BASH_SOURCE-$0}`
. env.sh

echo IN START_CLIENTS $1 $2 $3 $4

LOG_DIR=$LOG_DIR/hl_exp_$NSERVERS"_"servers_$NTHREADS"_"threads_$TXRATE"_"rates
mkdir -p $LOG_DIR
i=0
for host in `cat $HOSTS`; do
	let n=i/2
	let i=i+1
	if [[ $n -eq $INDEX ]]; then
		cd $EXE_HOME
		if ! [ -e ./driver ]; then
			echo "Please compile the driver in $EXE_HOME"
			exit 1
		fi
		if [[ $BENCHMARK == 'ycsb' ]]; then
			nohup ./driver -db hyperledger -threads $NTHREADS -P workloads/workloada.spec -endpoint $host:7050/chaincode -txrate $TXRATE > $LOG_DIR/client_$host"_"$NTHREADS 2>&1 &
		elif [[ $BENCHMARK == 'smallbank' ]]; then
			nohup ./driver 10000000 $NTHREADS 1000 $LOG_DIR/stat.log $host:7050/chaincode > $LOG_DIR/client_$host"_"$NTHREADS 2>&1 &
		elif [[ $BENCHMARK == 'donothing' ]]; then
			nohup ./driver -db hyperledger -threads $NTHREADS -P workloads/workloada.spec -endpoint $host:7050/chaincode -txrate $TXRATE -wl donothing > $LOG_DIR/client_$host"_"$NTHREADS 2>&1 &
		fi
	fi
done

