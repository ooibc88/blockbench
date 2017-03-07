# Experiment Setup

Need to populate the blockchain system using some pre-generated transactions before the analytical queries.
`txn_gen.js` shows an example about how to generate ~3 transactions/block.

# Run

* Q1: `node bench_q1.js start_block end_block` queries Q1 from block `start_block` to block `end_block`.

* Q2: `node bench_q2.js start_block end_block account` queries Q2 for account `account` from block `start_block` to block `end_block`.
