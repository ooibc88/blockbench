# Prepare
Download `peer` executable and make it accessible from `$PATH`
Spin up the [fabric network](../../../../benchmark/fabric-v1.4/four-nodes-docker/)

# Run
* use `./deploy.sh` to deploy the IOheavy smart contract to blockchain.
* use `./write.sh <start key> <# of records>` to measure the duration of batch writing. Both fields must be numeric. 
* use `./scan.sh <start key> <# of records>` to measure the duration of scanning. Both fields must be numeric. 
