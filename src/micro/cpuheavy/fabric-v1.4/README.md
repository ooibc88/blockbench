# Prepare
Download `peer` executable and make it accessible from `$PATH`
Spin up the [fabric network](../../../../benchmark/fabric-v1.4/four-nodes-docker/)

# Run
* use `./deploy.sh` to deploy the sorting smart contract to blockchain.
* use `./benchmark.sh <array_size>` to measure the duration. `array_size` is the size of the array to sort.
