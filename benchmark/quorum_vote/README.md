# Benchmarking Quorum

# Running multiple experiments using Python

To avoid manually changing settings in `evn.sh`, one can directly start multiple experiments using the
following Python scripts:

1. `config.py` contains several important global variables to be imported to other Python's scripts:
      + `NODES`: the list of strings representing the server IP addresses.
      + `parition_cmd`: use to specify the script that simulate parition attacks. 
      + `TIMEOUT`: 

1. `exps.py` 

2. `partition.py`

## Parameters
There are a number of global variables that are to be set in `env.sh`:
+ `$QUO_HOME`: NFS-mounted directory containing the scripts.
+ `$QUO_DATA`: non-NFS directory for Quorum data
+ `SHOSTS`: contains IP addresses of all nodes used for running the miners. The
example file contains 32 hosts 
+ `$CLIENTS`: containing IP addresses of all nodes used by the clients. There are 2 clients per
node. The example file contains 16 hosts
+ `$LOG_DIR`: directory where the logs are stored. NFS-mounted directories are recommended.  
+ `$EXE_HOME`: containing the executable for driving the workloads (ycsb or smallbank). For Quorum, smallbank
is simulated via ycsb, i.e. it is invoked via key-value operation but at the server side multiple read/write
operations are performed. 
    + YCSB: `$QUO_HOME/../../src/kystore`
    + Smallbank: `$QUO_HOME/../../src/smallbank`
+ `$BENCHMARK`: name of the benchmark (ycsb or smallbank)
+ `$QUORUM`: the directory of Quorum Binaries, which are placed in $QUORUM_REPO_ROOT/build/bin

Each network is initialized with different genesis block. There are a set of pre-defined genesis blocks for
different network sizes:

## Scripts
There are 4 steps in running an experiment: network initialization, miner startup, client startup, and finally
cleaning up. Scripts for these steps are included in `$QUO_HOME`. The top-level script:

    run-bench.sh <nservers> <nthreads> <nclients> [-drop]

will start `nservers` miners, `nclients` clients each running with `nthreads`. The outputs are stored in
`$LOG_DIR`. If `-drop` is specified, 4 servers will be killed after the clients are running for about 250s.  

### Initilization
+ `init-all.sh <nservers>`: go to each of `nservers` and invokes `init.sh <nservers>`
+ `init.sh <nservers>` initializes `quorum` at a local node. It does 3 things:
    + Use `keys/key$i` for the key of the static account
    + Use `$QUO_HOME/genesis_quorum.json` to as the genesis block
    + Use `$QUO_DATA` for `quorum` data 


### Starting voting
+ `start-all.sh <nservers>`: start a network of `nservers` miner in the following steps:
    1. Go to each of `nservers` node in `hosts` and invoke `start-mining.sh`. This involves:
        + Start `quorum` in vote mode

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
    1. Go to each server and invoke `stop.sh` which kills `quorum` and remove all Quorum data. 
    2. Go to each client (in `$QUO_HOME/clients`) and kill the `ycsbc` process. 

When the experiment exits cleanly (normal case), the client processes are already killed in
`start-multi-client.sh` and server processes in `run-bench.sh`. But if interrupted (Ctrl-C), both server and client processes should be killed
explicitly with `stop-all.sh`.


## Examples

1. Running with the same number of clients and servers: 
    + Change `LOG_DIR` in `env.sh` to correct location, say `result_same_s_same_c`
    + Start it (e.g. X=8): 

        `. run-bench.sh 8 16 8 16`

    This will start 8 miners (on first 8 nodes in `$HOSTS`) and 8 clients (on 4 first 4 nodes in `$CLIENTS`).
    Each client runs `driver` process with 16 threads. The clients output logs to
    `result_same_s_same_c/exp_8_servers` directory, with the file format `client_<miner_host>_16`.   

2. Running with fixed number of clients and varying number of servers: 
    + Change `LOG_DIR` in `env.sh` to correct location, say `result_fixed_c`
    + Start it (X=16): 

        `. run-bench.sh 16 16 8 16`

    This will start 16 miners (on first 16 nodes in `$HOSTS`) and 8 clients (on 4 first 4 nodes in
    `$CLIENTS`). Each client runs `driver` process with 16 threads. The clients output logs to
    `result_fixed_c/exp_16_servers` directory, with the file format `client_<miner_host>_16`.   


3. Drop off nodes:
    + Change `LOG_DIR` in `env.sh` to correct location, say `result_fixed_c_drop_4`
    + Start it (X=16): 

        `. run-bench.sh 16 16 8 16 -drop`

    This will start 16 miners (on first 16 nodes in `$HOSTS`) and 8 clients (on 4 first 4 nodes in
    `$CLIENTS`). Each client runs `driver` process with 16 threads. At 250th second, 4 miners are killed off,
    while the rest continues to run. The clients output logs to `result_fixed_c_drop_4/exp_16_servers`
      directory, with the file format `client_<miner_host>_16`.   

4. Running with different workloads:

Simply change `$EXE_HOME` and `$BENCHMARK` variables in `env.sh`. Then repeat the same steps as in the other
examples. 