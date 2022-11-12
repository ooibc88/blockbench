#!/bin/bash
cd `dirname ${BASH_SOURCE-$0}`
. env.sh

killall -KILL geth
rm -rf $ETH_DATA
rm -rf ~/.eth*
