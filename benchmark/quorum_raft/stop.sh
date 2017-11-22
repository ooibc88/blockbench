#!/bin/bash
cd `dirname ${BASH_SOURCE-$0}`
. env.sh

echo "stop.sh" 
killall -KILL geth ${QUORUM}
rm -rf $QUO_DATA
#rm -rf $QUO_DATA/chaindata/
rm -rf ~/.eth*
