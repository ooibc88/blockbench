# Experiment Setup

Need to deploy the ioheavy chaincode.
The source code of ioheavy chaincode is in `blockbench/benchmark/contracts/hyperledger/go/io/` directory as a reference.

1. use `node deploy.js` to initialize the chaincode into Hyperledger.

# Run

* Write: `node write.js start_key total_key_num` write `total_key_num` key-value pairs starting with key `start_key` sequentially.
* Scan: `node scan.js star_key total_key_num` scan `total_key_num` key-value pairs starting with key `start_key` sequentially.
