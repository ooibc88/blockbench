#!/bin/bash
cd `dirname ${BASH_SOURCE-$0}`
. env.sh
echo "start-addPeer.sh"

for com in `cat $QUO_HOME/addPeer.txt`; do
  ${QUORUM} --exec $com attach ipc:/$QUO_DATA/geth.ipc
done
