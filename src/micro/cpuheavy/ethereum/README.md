# Prepare
At first, use `npm start` or [start_ethereum.sh](../../ethereum_script/start_ethereum.sh) to start geth client, then open another terminal to do next step

# Run
* use `node deploy.js array_size` to deploy the sorting smart contract to blockchain, and also issue a sort request. It will report the latency into `stdout`. 
 `array_size` is the size of the array to sort.
