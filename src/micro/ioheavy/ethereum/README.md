# Prepare
At first, use `npm start` or [start_ethereum.sh](../../ethereum_script/start_ethereum.sh) to start geth client, then open another terminal to do next step

# Experiment Setup

Need to deploy the ioheavy smart contract first.
The source code of ioheavy smart contract source code in Solidity is `blockbench/benchmark/contracts/ethereum/ioheavy.sol` as a reference.

* use `node deploy.js` to deploy the smart contract into blockchain(ethereum/parity). It will output the deployed smart contract address
into `stdout`.

# Run

* Write: `node write.js start_key total_key_num sig contract_address` write `total_key_num` key-value pairs starting with key `start_key` sequentially.
  `sig` is a id stamp of this request used for distinguish this request from the others (due to some web3.js library bugs, this is needed). `contract_address`
  is the deployed smart contract address returned by `deploy.js`.
* Scan: `node scan.js star_key total_key_num sig contract_address` scan `total_key_num` key-value pairs starting with key `start_key` sequentially.
  `sig` is a id stamp of this request used for distinguish this request from the others (due to some web3.js library bugs, this is needed). `contract_address`
  is the deployed smart contract address returned by `deploy.js`.
