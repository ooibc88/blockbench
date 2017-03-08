# Experiment Setup

Need to deploy the analytics chaincode & populate the blockchain system using some pre-generated transactions before the analytical queries.
The source code of analytics chaincode is in `blockbench/benchmark/contracts/hyperledger/go/analytic/` directory.

1. use `node deploy.js account_number` to initialize the chaincode into Hyperledger. `account_number` indicates how many initial accounts
are created in the state of this chaincode.

2. use `node populate.js block_number` to populate the chaincode state for `block_number` of blocks. This script will generate transactions
using random accounts.

# Run

* Q1: `node bench_q1.js start_block end_block` queries Q1 from block `start_block` to block `end_block`.

* Q2: `node bench_q2.js account start_block end_block` queries Q2 for account `account` from block `start_block` to block `end_block`. `account` is an integer.
