# Running a sample network
Launch a simple network with a single orderer and two peers from different organizations. 
## Download 
Download binaries and pull docker images for Fabric v2.2 based on this [link](https://hyperledger-fabric.readthedocs.io/en/latest/install.html). 
Make sure the downloaded binaries are accessible from `$PATH`.

## Set Global Variable
```
CHANNEl_NAME=mychannel
```

## Launch and setup
```
./network.sh up createChannel -ca -i 2.2 -c ${CHANNEl_NAME}
```

## Deploy Chaincode
The exact `$CC_NAME` is determined by individual benchmarks. 
```
CC_SRC_PATH="../contracts/fabric-v2.2/${CC_NAME}"
./network.sh deployCC -ccn ${CC_NAME} -ccp ${CC_SRC_PATH}
```

## Start up the helper processes.
This part is necessary as Fabriv 2.2 does not provide C++ SDK. To work around, we wrap up the Fabric's service into a json web service, implemented by NodeJS. In macro benchmarks, client drivers will contact these helpers processes to interact with Fabric network. 

Note down their listening addresses and ports. 

### Install Dependencies and Prepare Identities
```
cd services/;
npm install;
node enrollAdmin.js
node registerUser.js
```

### Launch processes

Launch one block service on port 8800 in the background, which provides the ledger information. 

Since __block-server__ is only contacted by the __Status_Thread__ of client drivers, a single one block service is enough. 
```
# Still in services/
node block-server.js ${CHANNEl_NAME} 8800 > block-server.log 2>&1 &
```

Launch two transaction services on port 8801 and 8802 respectively in the background, each listening for json requests and routes them to Fabric network. 
```
# Still in services/
node txn-server.js ${CHANNEl_NAME} ${CC_NAME} 8801 > txn-server-8801.log 2>&1 &
node txn-server.js ${CHANNEl_NAME} ${CC_NAME} 8802 > txn-server-8802.log 2>&1 &
```
As __txn-servers__ are contacted by the __Client_Thread__ of client drivers, and there are multiple client threads, it is desirable to launch multiple __txn-servers__ to balance the workload. 

# Clean up
## Shut the network
```
./network.sh down
```

## Shut the helper processes
```
ps aux  |  grep -i block-server  |  awk '{print $2}' | xargs kill -9
ps aux  |  grep -i txn-server  |  awk '{print $2}' | xargs kill -9
```

## Clean the identity
```
## Under services/
rm wallet/*
```
