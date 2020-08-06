# Prepare
Download `peer` executable and make it accessible from `$PATH`
Spin up the [fabric network](../../../../benchmark/fabric-v1.4/four-nodes-docker/)

# Run
## Deploy
* use `./deploy.sh` to deploy the Analytic smart contract to blockchain.

## Install NodeJS SDK
`npm install`

## Populate Transactions
`node populate.js <orderer-address> <peer-address> <#-of-accounts> <#-of-txns>`
If `#-of-accounts=100`, 100 acounts named with *Acc0*, *Acc1*, ..., *Acc99* are created. 

## Query Block Height
`node query_blk_height.js <orderer-address> <peer-address>`

## Benchmark Q1
`node bench_q1.js <orderer-address> <peer-address> <start-block> <end-block>`

## Benchmark Q2
`node bench_q2.js <orderer-address> <peer-address> <start-block> <end-block> <account-name>`
