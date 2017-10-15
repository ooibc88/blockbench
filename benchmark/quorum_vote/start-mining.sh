#!/bin/bash
cd `dirname ${BASH_SOURCE-$0}`
. env.sh
echo "start-mining.sh"

if [[ $1 -lt 1 ]]; then
	echo "blockmaker"
	cp keys/key* $QUO_DATA/keystore
	nohup ${QUORUM} --datadir $QUO_DATA --rpccorsdomain "*" --nodiscover --voteaccount 0x981010def42136f95ad6f302052e660bac5f2b3f --votepassword "" --blockmakeraccount 0xeb6902c98ed64b7b16f743a7c09b36b8fa1aff59  --blockmakerpassword "" --rpc --rpcaddr 0.0.0.0 --rpcport 8000 --singleblockmaker --minblocktime 1 --maxblocktime 3 > $QUO_DATA/../vote_quorum_log 2>&1 &
else 
	i=0
	for address in `cat $ADDRESSES`; do
 		if [[ $i -eq $1 ]]; then
			nohup ${QUORUM} --datadir $QUO_DATA --rpccorsdomain "*" --nodiscover --voteaccount $address --votepassword "" --rpc --rpcaddr 0.0.0.0 --rpcport 8000 > $QUO_DATA/../vote_quorum_log 2>&1 &
   		echo done node $host
  		fi
  	let i=$i+1
	done
fi
