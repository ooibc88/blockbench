# Macro benchmark workloads
This directory contains the drivers for macro benchmark workload - YCSB and SmallBank.

The DoNothing micro benchmark workload which is targeted to test the consensus layer is also integrated into the ycsbc-based driver.

The driver in `kvstore` directory provides the following workloads:

* KVStore: for Ethereum, Parity , Quorum and Hyperledger

  Usage example:
  ```
  ./driver -db parity -threads 1 -P workloads/workloada.spec -txrate 5 -endpoint localhost:8545 -wl ycsb -wt 20
  ```

* KVStore: for Fabric 2.2
  * Spin up the network, create the channel, deploy the contract and launch the helper web services, as in [README](../../benchmark/fabric-v2.2/README.md) with the following two bash **variables** `CC_NAME=kvstore MODE=open_loop`.
  * Prepare the endpoint `endpoint=[block-service-address],[txn-service-address1],[txn-service-address2]...`. Based on the configuration in [README](../../benchmark/fabric-v2.2/README.md) and assume helper services are run in the same machine as the client driver, `endpoint=localhost:8800,localhost:8801,localhost:8802`
  * Launch the client driver processes. Each client thread will randomly contact one of the transaction web service to invoke a transaction or make a query. 
  Usage example:
  ```
  ./driver -db fabric-v2.2 -threads 1 -P workloads/workloada.spec -txrate 5 -endpoint localhost:8545 -wl ycsb -wt 20
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
  ./driver -db hyperledger -threads 1 -txrate 5 -P workloads/workloada.spec -endpoint localhost:7050/chaincode -wl donothing

* DoNothing: for Fabric 2.2
  * Repeat the above step as in KVStore, except that `$CC_NAME=donothing`.
  * Usage: 
  ```
  ./driver -db fabric-v2.2 -threads 1 -txrate 5 -P workloads/workloada.spec -endpoint ${endpoint} -wl donthing -wt 20
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

* SmallBank: for Fabric 2.2
  * Repeat the above step as in KVStore, except that `$CC_NAME=smallbank`.
  * Usage: 
  ```
  ./driver -db fabric-v2.2 -ops 1000 -threads 10 -txrate 10 -fp stat.txt -endpoint ${endpoint}
  ```
  * Note: In Smallbank workload, one may observe that the number of outstanding transactions are constantly increasing. It is because some Smallbank transactions may be classified as invalid due to the read-write conflict. The block web service will not return these invalid transactions. Hence, these transactions forever stay in the pending queue. 

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

