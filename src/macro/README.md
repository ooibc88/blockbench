# Macro benchmark workloads
This directory contains the drivers for macro benchmark workload - YCSB and SmallBank.

The DoNothing micro benchmark workload which is targeted to test the consensus layer is also integrated into the ycsbc-based driver.

The driver in `kvstore` directory provides the following workloads:

* KVStore: for Ethereum, Parity , Quorum and Hyperledger

  Usage example:
  ```
  ./driver -db parity -threads 1 -P workloads/workloada.spec -txrate 5 -endpoint localhost:8545 -wl ycsb -wt 20
  ```

* KVStore: for Fabric 1.4
  * Spin up the network in docker mode, as in [README](../../benchmark/fabric-v1.4/README.md)
  * Enter into [fabric-v1.4-node](kvstore/fabric-v1.4-node) and install nodejs dependency with shell cmd:
  ```
  cd src/macro/kvstore/fabric-v1.4-node
  npm install
  ```
  * Deploy the smart contract (chaincode) via the bash script, `peer` process must be accessible from `$PATH`
  ```
  # Assume under src/macro/kvstore/fabric-v1.4-node
  ./deploy_kv.sh # with respect to the above docker environment
  ```
  * Run the driver to benchmark. Usage example: 
  ```
  ./driver -db fabric-v1.4 -threads 16 -P workloads/workloada.spec -txrate 100 -endpoint <orderer_addr>,<peer_addr> -wl ycsb
  ```
  For example, if the driver is executed in the same machine as the above docker network, the command would be like: 
  ```
  ./driver -db fabric-v1.4 -threads 16 -P workloads/workloada.spec -txrate 100 -endpoint localhost:7041,localhost:7051 -wl ycsb
  ```

* DoNothing: for Ethereum, Parity , Quorum and Hyperledger

  Usage example:
  ```
  ./driver -db hyperledger -threads 1 -P workloads/workloada.spec -txrate 5 -endpoint localhost:7050/chaincode -wl donothing
  ```

* DoNothing: for Fabric 1.4
  * Spin up the network in docker mode, as in [README](../../benchmark/fabric-v1.4/README.md)
  * Enter into [fabric-v1.4-node](kvstore/fabric-v1.4-node) and install nodejs dependency with shell cmd:
  ```
  cd src/macro/kvstore/fabric-v1.4-node
  npm install
  ```
  * Deploy the smart contract (chaincode) via the bash script, `peer` process must be accessible from `$PATH`
  ```
  # Assume under src/macro/kvstore/fabric-v1.4-node
  ./deploy_donothing.sh # with respect to the above docker environment
  ```
  * Run the driver to benchmark. Usage example: 
  ```
  ./driver -db fabric-v1.4 -threads 16 -P workloads/workloada.spec -txrate 100 -endpoint <orderer_addr>,<peer_addr> -wl donothing
  ```
  For example, if the driver is executed in the same machine as the above docker network, the command would be like: 
  ```
  ./driver -db fabric-v1.4 -threads 16 -P workloads/workloada.spec -txrate 100 -endpoint localhost:7041,localhost:7051 -wl donothing
  ```

The driver in `smallbank` directory provides the following workloads:

* SmallBank: for Ethereum, Parity , Quorum and Hyperledger 

  Usage example:
  ```
  ./driver -db ethereum -ops 10000 -threads 4 -txrate 10 -fp stat.txt -endpoint 127.0.0.1:8000 
  ```

* Smallbank: For Fabric v1.4
  * Spin up the network in docker mode, as in [README](../../benchmark/fabric-v1.4/README.md)
  * Enter into [fabric-v1.4-node](smallbank/api_adapters/fabric-v1.4-node) and install nodejs dependency with shell cmd:
  ```
  cd src/macro/smallbank/api_adapters/fabric-v1.4-node
  npm install
  ```
  * Deploy the smart contract (chaincode) via the bash script
  ```
  # Assume under src/macro/smallbank/api_adapters/fabric-v1.4-node
  ./deploy.sh # with respect to the above docker environment
  ```
  * Run the driver to benchmark. Usage example: 
  ```
  ./driver  -db fabric-v1.4 -ops 1000 -threads 4 -txrate 100 -fp stat.txt -endpoint <orderer_addr>,<peer_addr>
  ```
  For example, if the driver is executed in the same machine as the above docker network, the command would be like: 
  ```
  ./driver  -db fabric-v1.4 -ops 1000 -threads 4 -txrate 100 -fp stat.txt -endpoint localhost:7041,localhost:7051
  ```

