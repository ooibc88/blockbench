# Macro benchmark workloads
This directory contains the drivers for macro benchmark workload - YCSB and SmallBank.

The DoNothing micro benchmark workload which is targeted to test the consensus layer is also integrated into the ycsbc-based driver.

The driver in `kvstore` directory provides the following workloads:

* KVStore: for Ethereum, Parity , Quorum and Hyperledger

  Usage example:
  ```
  ./driver -db parity -threads 1 -P workloads/workloada.spec -txrate 5 -endpoint localhost:8545 -wl ycsb -wt 20
  ```

* DoNothing: for Ethereum, Parity , Quorum and Hyperledger

  Usage example:
  ```
  ./driver -db hyperledger -threads 1 -P workloads/workloada.spec -txrate 5 -endpoint localhost:7050/chaincode -wl donothing
  ```

The driver in `smallbank` directory provides the following workloads:

* SmallBank: for Ethereum, Parity , Quorum and Hyperledger 

  Usage example:
  ```
  ./driver -db ethereum -ops 10000 -threads 4 -txrate 10 -fp stat.txt -endpoint 127.0.0.1:8000 
  ```
