# Micro benchmark workloads
A blockchain system comprises multiple layers, and each layer may have different impact on the overall performance.
We provide several workloads to stress the layers in order to understand their individual performance.

## Workloads
* Consensus layer - DoNothing
  This workload is integrated in the ycsbc-based driver in `blockbench/src/macro/kvstore` directory.

  To use this workload,
  ```
  # Hyperledger
  ./driver -db hyperledger -threads 1 -P workloads/workloada.spec -txrate 5 -endpoint localhost:7050/chaincode -wl donothing
  # Ehtereum
  ./driver -db ethereum -threads 1 -P workloads/workloada.spec -txrate 5 -endpoint localhost:8545 -wl donothing -wt 20
  # Parity
  ./driver -db parity -threads 1 -P workloads/workloada.spec -txrate 5 -endpoint localhost:8545 -wl donothing -wt 20
  ```

* Data model layer - IOHeavy & Analytics

  * [IOHeavy](ioheavy/README.md) workload description.
  * [Analytic](analytic/README.md) workload description.


* Execution layer - CPUHeavy

  [CPUHeavy](cpuheavy/README.md) workload description.

## Node.js dependency libraries 
* Use `npm install` to install the dependency libraries
* For all ethereum experiments, use [start_ethereum.sh](ethereum_script/start_ethereum.sh) to start geth client first.
