# CPUHeavy workload for Hyperledger Fabric v1.0-RC1

## Contents

`chaincode` folder contains the cpuheavy chaincode using the latest version Chaincode StubAPI.

`network` folder contains keyfiles which are gotten directly from the Fabric example https://github.com/hyperledger/fabric-samples/tree/master/fabcar

`run-peer.sh` & `run-orderer.sh` set some system environment variables before start the `peer` and `orderer` executable binary

`create_channel.sh`, `join_channel.sh`, `install_chaincode.sh`, `instantiate_chaincode.sh` are scripts to instantiate the chaincode.

`invoke.js` is the chaincode driver which issues transactions using Fabric Node.js SDK, `invoke.sh` is a script to launch the driver and this workload measures the efficiency of the execution layer for computationally heavy tasks.
The smart contract initializes a large array, and runs the quick sort algorithm over it.

## Running steps

### Step 1: Compile Fabric v1.0-RC1

Download source code from https://github.com/hyperledger/fabric/releases and choose the v1.0.0-rc1 tag.
Or use source code with UStore support from https://github.com/ijingo/fabric/ under the `1.0-rc1-ustore` 
branch.

**Put the source code under `$GOPATH`, such as `$GOPATH/src/github.com/hyperledger/fabric/`**

Under the project root path (`$GOPATH/src/github.com/hyperledger/fabric`), use `make peer` and `make orderer`.
This will download base docker images and generate binaries into the `$GOPATH/src/github.com/hyperledger/fabric/build/bin/` directory.

### Step 2: Setup a test network

0. Copy all the things from this repository into the `$GOPATH/src/github.com/hyperledger/fabric/build/bin` directory.

1. Go to `cpuheavy` directory and use `npm install` to download the nodejs module.

2. Add 
```
127.0.0.1 orderer.example.com
127.0.0.1 peer0.example.com
127.0.0.1 peer1.example.com
```
into `/etc/hosts`

3. Edit the path prefix of the line `export CORE_PEER_MSPCONFIGPATH=/data/wangji/gowork/src/github.com/hyperledger/fabric/build/bin/network/crypto-config/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/msp` in `run-peer.sh` accordingly, and change the line `export CORE_PEER_FILESYSTEMPATH=/data/wangji/tmp/hyperledger/production` to make it point to the desired folder to hold the ledger data.

Use `export CORE_LEDGER_STATE_STATEDATABASE=goleveldb` in the `run-peer.sh` to enable goleveldb as the storage.

Also, edit the path prefix of the line `export ORDERER_GENERAL_GENESISFILE=/data/wangji/gowork/src/github.com/hyperledger/fabric/build/bin/network/genesis.block` and `export ORDERER_GENERAL_LOCALMSPDIR=/data/wangji/gowork/src/github.com/hyperledger/fabric/build/bin/network/crypto-config/ordererOrganizations/example.com/orderers/orderer.example.com/msp` in `run-orderer.sh` accordingly, also make `export ORDERER_FILELEDGER_LOCATION=/data/wangji/tmp/hyperledger/production/orderer` point to the storage location.

This step is to set the `peer` and `orderer` using the private keys in the `network/crypto-config` folder, and also set the data storage path of the ledger data when using `goleveldb` as the storage.

4. Run `run-orderer.sh` to launch an `orderer` and run `run-peer.sh` to launch a `peer`.

5. Change the `CORE_PEER_MSPCONFIGPATH` environment variable in the `*_channel.sh` and `*_chaincode.sh` as well. Change the `mychannel.tx` file path in the `create_channel.sh` file  (last line in this file). Then run
`create_channel.sh`, `join_channel.sh` sequentially.

### Step 3: Install chaincode

Create a folder under `$GOPATH/src/github.com/` named `cpuheavy`, copy `chaincode/cpuheavy.go` file into `$GOPATH/src/github.com/cpuheavy` folder. Go to `cpuheavy` directory. Run `install_chaincode.sh` and `instantiate_chaincode.sh` to instantiate a chaincode instance.

Note: in version 1.0, the chaincode is no need to be downloaded from Github.

### Step 4: Launch workload driver

Use `node invoke.js array_size` to issue a sort request to Hyperledger, `array_size` is the size of the array to sort.

The node.js driver will automatically registers an event to wait until the transaction finish.


## Test Environment

* ciidaa-a19 machine
* node.js == v6.11.0
* go == 1.8.3
* cmake == 3.8.2
* Docker version 17.03.2-ce, build f5ec1e2
