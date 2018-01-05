# Prepare
At first, use `npm start` or [start_ethereum.sh](../../ethereum_script/start_ethereum.sh) to start geth client, then open another terminal to do next step

# Experiment Setup

Need to populate the blockchain system using some pre-generated transactions before the analytical queries.
`txn_gen.js` shows an example about how to generate ~3 transactions/block.

# Run

* Q1: `node bench_q1.js start_block end_block` queries Q1(total transacation value) from block `start_block` to block `end_block`.

* Q2: `node bench_q2.js start_block end_block account` queries Q2(largest transaction value) for account `account` from block `start_block` to block `end_block`.(for example: `node bench_q2.js 3 100 0x12f029d57082315085bfb4d4d8345c92c5cdd881`)

* Q3: `node bench_q3.js start_block end_block account` queries Q3(largest account balance) for account `account` from block `start_block` to block `end_block`.(for example: `node bench_q3.js 3 100 0x12f029d57082315085bfb4d4d8345c92c5cdd881`)
