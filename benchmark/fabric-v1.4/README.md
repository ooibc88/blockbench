# Prerequisite
* Install [docker](https://www.docker.com/) and [docker-compose](https://docs.docker.com/compose/) tools

# Running Fabric v1.4

* Spin up a Fabric network with a single orderer and four peers under the same organization. 
* Run the network in the docker environment of the localhost in the same bridge network. 

```
cd four-nodes-docker/
docker-compose -f docker-compose.yaml up -d
./setup.sh # create the channel and join the channel for each peer
```

# Benchmark
* Enter the workload directory to benchmark against this setup. 

# Shut down
```
docker-compose -f docker-compose.yaml down
```



