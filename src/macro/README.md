# Macro benchmark workloads
This directory contains the drivers for macro benchmark workload - YCSB and SmallBank.

The DoNothing micro benchmark workload which is targeted to test the consensus layer is also integrated into the ycsbc-based driver.

The driver in `kvstore` directory provides the following workloads:

* KVStore: for Ethereum, Parity and Hyperledger

  Usage example:
  ```
  ./driver -db parity -threads 1 -P workloads/workloada.spec -txrate 5 -endpoint localhost:8545 -wl ycsb -wt 20
  ```

* SmallBank: for Ethereum and Parity 

  Usage example:
  ```
  ./driver -db etheruem -threads 1 -P workloads/workloada.spec -txrate 5 -endpoint localhost:8545 -wl smallbank -wt 20
  ```

* DoNothing: for Ethereum, Parity and Hyperledger

  Usage example:
  ```
  ./driver -db hyperledger -threads 1 -P workloads/workloada.spec -txrate 5 -endpoint localhost:7050/chaincode -wl donothing
  ```

The `smallbank` directory contains the driver of SmallBank workload for Hyperledger
