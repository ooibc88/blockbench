# Manual deployment of Parity on a cluster

## Generate config files
First, we need to generate a list of config files for launching the nodes.

      `python gen_config.py <#nodes>` 
      
will generate the following files:

+ `chain_spec.json`: specification of the genesis chain, which include list of validators, etc. 
+ `config.tom.[1..<#nodes>]`: multiple config files each contain network parameter for the node, such as who
are the signer, mining parameters, etc. 

## Other important files
+ `keys`: directory contains all the keys for the accounts
+ `node.pwds`: password strings

For more details, see https://github.com/ethcore/parity/wiki/Proof-of-Authority-Chains

## Launch Parity server

      `./start_parity.sh <data dir> <host/IP> <log dir> <parity binary> <host index>`

where:
+ `<data dir>`: directory where chain data + keys are store. Make sure it is non-NFS
+ `<host/IP`: IP address of the host. The RPC end point will be <hosts/IP>:8545
+ `<parity binary>`: where the Parity executable is
+ `<host index>`: from 1 to `<#nodes>`

## Add peers to the network

This step is needed to get the Parity nodes connected to each other. There are several steps needed (best to
use the script, described later):

1. Send a query to each running node, asking for its `enode`, using `parity_enode` method
2. Gather all `enode` values, send each value to every node, using `parity_addReservedPeer` method

At the end, the nodes should learn about each other's presence. 

## Launch a client:

Either YCSB or Smallbank comes with a driver for Parity, and the launch command is the same as in Ethereum.

      `.../driver -db parity -threads <#threads> -P <workload> -endpoint <host:port> -txrate <rate>`

---

# Deployment using a script

The steps above are tedious and error prone. The following scripts help automate the whole thing

### Config file

The file `config.py` is **very important**, as it defines global values, namely:
+ `NS`: number  of nodes (servers)
+ `NODES`, `CLIENTS`: list of IP addresses of the servers and of the clients
+ `THREADS`, `RATES`, `[EXPS]`: number of threads and tx rate. These 2 can be replaced by a list of tuples
representing `(#threads, rate)` to be run. 
+ `HOME_DIR`: the current directory containing this scripts and other
+ `DATA_DIR`, `LOG_DIR`: data and log directory for the **servers**
+ `PARITY_EXE`: where the Parity executable is
+ `CLIENT_LOG`: directory where the client's logs go
+ `enode_command`, `add_peer`, `partition_cmd`, `kill_cmd`: string formatted OS commands
+ `TIMEOUT`: applicable to the security benchmark

### Run scripts
The file `run.py` drives all experiments. 

      `python run.py <start/kill> [-security/-fixed/-drop/-smallbank/-donothing]`

It consists of the following important functions:
+ `start_parity(.)`: ssh to every node in `NODES` and execute the `start_parity.sh` command with the
appropriate parameters.  

+ `get_enodes(.)`: send `parity_enode` RPC request to every node, return a list of `enode://` strings

+ `add_peers(.)`: send `parity_addReservedPeer` request to every node, for every `enode://` string in the list

+ `start_clients(.)`: loop through the client-server tuple and invoke `start_xxx_client.sh` with the
appropriate workload (default is YCSB) and parameters. 

+ `partition(.)`: imported from `partition.py`, which partitions `NODES` in half for a given duration. 

+ `run_exps(.)`: main driver, which start servers and client. For the security benchmark, it then waits for
100 seconds before invoking partitioning. Finally, it waits for a fixed duration (500 seconds in total) before
killing clients and server processes. 

---

# Run Parity benchmarks

Each benchmark has it own configuration file:
+ `config_[ycsb/smallbank/donothing].py`: for [YCSB/Smallbank/DoNothing] benchmark, fixed number of clients and server
+ `config_saturation.py`: for the saturation benchmark, using YCSB
+ `config_security.py`: for the security benchmark, using YCSB
+ `config_scale.py`: for the scale benchmark with the same number of servers and clients, using YCSB
+ `config_scale_fix_8.py`: for the scale benchmark with fixed number of clients (8), using YCSB
+ `config_drop.py`: for the failure benchmark, using YCSB

It is important to change settings in these files before running the corresponding benchmarks. 

## Run all benchmarks in one go
Having set the config files, we can run all benchmarks in one go:

      `python run_all.py`


## Run individual benchmark
To run benchmark XXX, there are 2 steps:

1. Copy the benchmark's config file to `config.py`. That is: 
      `cp config_XXX.py config.py`

2. Start the benchmark:
      `python run.py start [opts]`

Without [opts], this will execute YCSB workload for whatever specified in the `config.py` file (by default, it
is running with fixed/8 servers and clients). 

