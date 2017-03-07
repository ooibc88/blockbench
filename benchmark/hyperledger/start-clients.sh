#!/bin/bash
# args=THREADS index N txrate
cd `dirname ${BASH_SOURCE-$0}`
. env.sh

echo IN START_CLIENTS $1 $2 $3 $4
LOG_DIR=$LOG_DIR/hl_exp_$3"_"servers_$1"_"threads_$4"_"rates
mkdir -p $LOG_DIR
i=0
for host in `cat $HOSTS`; do
  let n=i/2
  let i=i+1
  if [[ $n -eq $2 ]]; then
    cd $EXE_HOME
    if [[ $BENCHMARK == 'ycsb' ]]; then
      nohup ./driver -db hyperledger -threads $1 -P workloads/workloada.spec -endpoint $host:7050/chaincode -txrate $4 > $LOG_DIR/client_$host"_"$1 2>&1 &
    elif [[ $BENCHMARK == 'smallbank' ]]; then
      nohup ./driver 10000000 $1 1000 /data/dinhtta/stat.log $host:7050/chaincode > $LOG_DIR/client_$host"_"$1 2>&1 &
    fi
  fi
done

