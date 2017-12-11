# Benchmarking Ethereum

There are Python scripts available for running the following experiments:

1. [Saturation experiments:](saturation_exps.md) with varying client request rate. 

2. [Security experiments:](security_exps.md) attacks are simulated via network partition. 

3. [DoNothing experiments:](donothing_exps.md) where transactions return right away. 

The experiments results are in `/data/dinhtta/blockbench_exps_results/ethereum' in the head node. 

## Parameters
There are a number of global variables that are to be set in `env.sh`:
+ `$ETH_HOME`: NFS-mounted directory containing the scripts.
+ `$ETH_DATA`: non-NFS directory for Ethereum data
+ `SHOSTS`: contains IP addresses of all nodes used for running the miners. The
example file contains 32 hosts 
+ `$CLIENTS`: containing IP addresses of all nodes used by the clients. There are 2 clients per
node. The example file contains 16 hosts
+ `$LOG_DIR`: directory where the logs are stored. NFS-mounted directories are recommended.  
+ `$EXE_HOME`: containing the executable for driving the workloads (ycsb or smallbank). For Ethereum, smallbank
is simulated via ycsb, i.e. it is invoked via key-value operation but at the server side multiple read/write
operations are performed. 
    + YCSB: `$ETH_HOME/../src/ycsb`
    + Smallbank: `$ETH_HOME/../src/smallbank/ycsb`
+ `$BENCHMARK`: name of the benchmark (ycsb or smallbank)

Each network is initialized with different genesis block. There are a set of pre-defined genesis blocks for
different network sizes:

+ `$ETH_HOME/CustomGenesis_<nservers>.json`: contains the genesis block for experiments using `nservers`. 
    + Hash rate is controlled by `difficulty` (higher it is, the lower the rate)
    + Block size is controlled by `gasLimit` (higher it is, the bigger the block)

## Scripts
There are 4 steps in running an experiment: network initialization, miner startup, client startup, and finally
cleaning up. Scripts for these steps are included in `$ETH_HOME`. The top-level script:

    run-bench.sh <nservers> <nthreads> <nclients> <txrate> [-drop]

will start `nservers` miners, `nclients` clients each running with `nthreads`, each thread issuing `txrate` transactions
per second. The outputs are stored in
`$LOG_DIR`. If `-drop` is specified, 4 servers will be killed after the clients are running for about 250s.  

### Initilization
+ `init-all.sh <nservers>`: go to each of `nservers` and invokes `init.sh <nservers>`
+ `init.sh <nservers>` initializes `geth` at a local node. It does 2 things:
    + Use `$ETH_HOME/CustomGenesis_<nservers>.json` to as the genesis block
    + Create new account with empty password
    + Use `$ETH_DATA` for `geth` data 

### Starting miners
+ `start-all.sh <nservers>`: start a network of `nservers` miner in the following steps:
    1. `gather.sh <nservers>`: go to each of `nservers` node in `hosts` to:
        + Collect peer information (of the miner) generated during the initialization step. This is done using
        `enode.sh` script
        + Add such information to a global, shared `addPeer.txt` file
    2. Go to each of `nservers` node in `hosts` and invoke `start-mining.sh`. This involves:
        + Start `geth` in mining mode, with configurable options such as `--maxpeer, --gasprice,
        --minerthreads` 
        + Add peers to each `geth` server, using the content in `addPeer.txt`

+ Sleep for a duration `M` (seconds) sufficient for all miners to (1) finish generating DAG, and (2) sync on
blockchain after mining few dozen of blocks. After tuning, the following during is found good enough:

    `M = 240 + 40*<nservers>`


### Starting the clients
+ `start-multi-clients.sh <nclients> <nservers> <nthreads> [-drop]`: takes as input a number of clients and
launch `2.nclients` clients connecting to `2.nclients` miners. It then performs the following:
    1. Go to each client node in `clients` and invoke `start-client.sh` 
        + `start-client.sh <nthreads> <client_index> <nservers>`: start `driver` process at the client node
       `client_index^{th}` to connect to the server node of index `2.(client_index)` and `2.(client_index)+1`
       
       **Different benchmark may expect different command line arguments for the `driver` process**

    2. Let the clients run for `M` (seconds), a sufficiently long duration to collect enough data. Then kill
    all client processes. Particularly:

        `M = 240 + 10*<nservers>`
    3. When `-drop` is specified:
        + It starts the clients and sleep for 250 seconds 
        + It then kills off the *last* 4 servers
        + It then continues to run (the remaining clients and servers) for another `(M-150)` seconds. 

    So in total, when `-drop` is specified, the entire experiments runs for about `(M+100)` seconds. 

### Cleaning up
+ `stop-all.sh <nservers>`: kill all server and client processes. Particularly:
    1. Go to each server and invoke `stop.sh` which kills `geth` and remove all Ethereum data. 
    2. Go to each client (in `$ETH_HOME/clients`) and kill the `ycsbc` process. 

When the experiment exits cleanly (normal case), the client processes are already killed in
`start-multi-client.sh` and server processes in `run-bench.sh`. But if interrupted (Ctrl-C), both server and client processes should be killed
explicitly with `stop-all.sh`.


## Examples

1. Running with the same number of clients and servers: 
    + Ensure `CustomGenesis_X.json` have the right difficulty level
    + Change `LOG_DIR` in `env.sh` to correct location, say `result_same_s_same_c`
    + Start it (e.g. X=8): 

        `. run-bench.sh 8 16 8 10`

    This will start 8 miners (on first 8 nodes in `$HOSTS`) and 8 clients (on 4 first 4 nodes in `$CLIENTS`).
    Each client runs `driver` process with 16 threads, each thread issuing 10 transactions per second. The clients output logs to
    `result_same_s_same_c/exp_8_servers` directory, with the file format `client_<miner_host>_16`.   

2. Running with fixed number of clients and varying number of servers: 
    + Ensure `CustomGenesis_X.json` have the right difficulty level
    + Change `LOG_DIR` in `env.sh` to correct location, say `result_fixed_c`
    + Start it (X=16): 

        `. run-bench.sh 16 16 8 10`

    This will start 16 miners (on first 16 nodes in `$HOSTS`) and 8 clients (on 4 first 4 nodes in
    `$CLIENTS`). Each client runs `driver` process with 16 threads. The clients output logs to
    `result_fixed_c/exp_16_servers` directory, with the file format `client_<miner_host>_16`.   


3. Drop off nodes:
    + Ensure `CustomGenesis_X.json` have the right difficulty level
    + Change `LOG_DIR` in `env.sh` to correct location, say `result_fixed_c_drop_4`
    + Start it (X=16): 

        `. run-bench.sh 16 16 8 10 -drop`

    This will start 16 miners (on first 16 nodes in `$HOSTS`) and 8 clients (on 4 first 4 nodes in
    `$CLIENTS`). Each client runs `driver` process with 16 threads. At 250th second, 4 miners are killed off,
    while the rest continues to run. The clients output logs to `result_fixed_c_drop_4/exp_16_servers`
      directory, with the file format `client_<miner_host>_16`.   

4. Running with different workloads:

Simply change `$EXE_HOME` and `$BENCHMARK` variables in `env.sh`. Then repeat the same steps as in the other
examples. 


# Running multiple experiments using Python

To avoid manually changing settings in `evn.sh`, one can directly start multiple experiments using the
following Python scripts:

1. `config.py` contains several important global variables to be imported to other Python's scripts:
      + `NODES`: the list of strings representing the server IP addresses.
      + `parition_cmd`: use to specify the script that simulate parition attacks. 
      + `TIMEOUT`: 

1. `exps.py` 

2. `partition.py`


## How to run security experiments

[Security experiments:](security_exps.md) attacks are simulated via network partition. 
