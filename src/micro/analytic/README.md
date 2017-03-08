# Micro Benchmark Workload - Analytic

## Description

This workload considers the performance of blockchain system in answering analytical queries about the
historical data. Similar to an OLAP benchmark, this workload evaluates how the system implements scan-like
and aggregate queries, which are determined by its data model. Specifically, we implement two queries for
extracting statistics from the blockchain data:

* Q1: Compute the total transaction values committed between block i and block j.

* Q2: Compute the largest transaction value involving a given account between block i and block j.

For Ethereum and Parity the queries are implemented using JSON-RPC. Ethereum and Parity share the same driver.
For Hyperledger we develop our own chaincode to implement the similar logic.
To see more details, please refer to the README file in each individual directory for each system.

## Details

+ [ethereum/parity](ethereum/README.md) analytic driver
+ [hyperledger](hyperledger/README.md) analytic driver
