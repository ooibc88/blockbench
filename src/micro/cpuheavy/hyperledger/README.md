# Experiment Setup

Need to deploy the CPUHeavy chaincode at first.
The source code of CPUHeavy chaincode is in `blockbench/benchmark/contracts/hyperledger/go/sorter/` directory as a reference.

* use `node deploy.js` to initialize the chaincode into Hyperledger.

# Run

* use `node invoke.js array_size` to issue a sort request to Hyperledger, `array_size` is the size of the array to sort.
