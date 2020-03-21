# Prerequisite
* Install [docker](https://www.docker.com/) and [docker-compose](https://docs.docker.com/compose/) tools
* Download `peer` binary from [here](https://github.com/hyperledger/fabric-samples/tree/release-1.4) and make it accessible from `$PATH`

# Running Fabric v1.4

* Spin up a Fabric network with a single orderer and four peers under the same organization. 
* Run the network in the docker environment of the localhost in the same bridge network. 

```
cd four-nodes-docker/
docker-compose -f docker-compose.yaml up -d
./setup.sh # create the channel and join the channel for each peer
```

# Benchmark
* Enter the corresponding workload directory, such as [smallbank](src/../../../src/macro/smallbank), to benchmark against this setup. 

# Shut down
```
docker-compose -f docker-compose.yaml down
```



