#!/bin/bash

cd `dirname ${BASH_SOURCE-$0}`
. env.sh

echo "Stopping nodes and clients..."
# stop peers
for peer in `cat $HOSTS`; do
  ssh $peer killall -KILL peer java driver
done

# stop clients
for client in `cat $CLIENTS`; do
  ssh $client killall -KILL driver peer java
done
