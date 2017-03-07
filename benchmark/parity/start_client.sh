#!/bin/bash
THREADS=$1
HOST=$2
LOG=$3
RATE=$4
cd `dirname ${BASH_SOURCE-$0}`

nohup ../../src/ycsb/driver -db parity -threads $THREADS -P ../../src/ycsb/workloads/workloada.spec -endpoint $HOST -txrate $RATE > $LOG 2>&1 &
