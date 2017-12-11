# Benchmarking Hyperledger

1. [Saturation experiments:](saturation_exps.md) with varying client request rate. 

2. [Security experiments:](security_exps.md) attacks are simulated via network partition. 

3. [DoNothing experiments:](donothing_exps.md) where transactions return right away. 

The experiments results are in `/data/dinhtta/blockbench_exps_results/hyperledger' in the head node. 


## Parameters
There are a number of global variables that are to be set in `env.sh`:
+ `$HL_HOME`: NFS-mounted directory containing the scripts.
+ `$HL_DATA`: non-NFS directory for Hyperledger data
+ `HL_SOURCE`: non-NFS directory containing the Hyperledger installation
+ `SHOSTS`: contains IP addresses of all nodes used for running validating peers. The
example file contains 32 hosts 
+ `$CLIENTS`: containing IP addresses of all nodes used by the clients. There are 2 clients per
node. The example file contains 16 hosts
+ `$LOG_DIR`: directory where the logs are stored. NFS-mounted directories are recommended.  
+ `$EXE_HOME`: containing the executable for driving the workloads (ycsb or smallbank). 
    + YCSB: `$ETH_HOME/../src/ycsb`
    + Smallbank: `$ETH_HOME/../src/smallbank/ycsb`
+ `$BENCHMARK`: name of the benchmark (ycsb or smallbank)

Since PBFT is used for consensus, each network size can tolerate only up to a certain number of failures. This
configuration is set in `$HL_SOURCE/consensus/pbft/config.yaml`. There are a set of pre-defined settings for
each different network sizes: 

+ `$HL_HOME/hl_consensus_<nservers>.yaml`: contains the genesis block for experiments using `nservers`. 
    + Number of peers `N`. 
    + Number of maximum failures `f` (must be no bigger than `N/3`). 
    
## Scripts
There are 3 steps in running an experiment: peer startup, client startup, and finally
cleaning up. Scripts for these steps are included in `$HL_HOME`. The top-level script:

    run-bench.sh <nservers> <nthreads> <nclients> <txrate> [-drop]

will start `nservers` peers, `nclients` clients each running with `nthreads`, each thread issuing `txrate` transactions
per second. The outputs are stored in
`$LOG_DIR`. If `-drop` is specified, 4 servers will be killed after the clients are running for about 250s.  

### Starting peers
+ `start-all.sh <nservers>`: start a network of `nservers` peers in the following steps:
    1. Copy the correct consensus configuration to each of the `nservers` peers. 
    2. Start the bootstrapping peer at the first host in `HOST`, by running 
    
        `start-root.sh`

    3. Start the remaining peers by running 
    
        `start-slave <bootstrapping_host> <peer_index>`     

Sleep for a few seconds in order for the peers to connect to each other (which is very fast). 

### Starting the clients
+ `start-multi-clients.sh <nclients> <nservers> <nthreads> [-drop]`: takes as input a number of clients and
launch `2.nclients` clients connecting to `2.nclients` servers. It then performs the following:
    1. Go to each client node in `clients` and invoke `start-client.sh` 
        + `start-client.sh <nthreads> <client_index> <nservers>`: start `driver` process at the client node
       `client_index^{th}` to connect to the server node of index `2.(client_index)` and `2.(client_index)+1`

       ** Different benchmark may expect different command line arguments for the `driver` process.**

    2. Let the clients run for `M` (seconds), a sufficiently long duration to collect enough data. Then kill
    all client processes. Particularly:

        `M = 320 + 10*<nservers>`
    3. When `-drop` is specified:
        + It starts the clients and sleep for 250 seconds 
        + It then kills off the *last* 4 servers
        + It then continues to run (the remaining clients and servers) for another `(M-250)` seconds. 

### Cleaning up
+ `stop-all.sh `: kill all server and client (driver) processes in all hosts listed in `$HOSTS` and `$CLIENTS`. 

When the experiment exits cleanly (normal case), the client processes are already killed in
`start-multi-client.sh` and server processes in `run-bench.sh`. But if interrupted (Ctrl-C), both server and
client processes should be killed explicitly with `stop-all.sh`.


## Examples

1. Running with the same number of clients and servers: 
    + Change `LOG_DIR` in `env.sh` to correct location, say `result_same_s_same_c`
    + Start it (e.g. X=8): 

        `. run-bench.sh 8 32 8 10`

    This will start 8 peers (on first 8 nodes in `$HOSTS`) and 8 clients (on 4 first 4 nodes in `$CLIENTS`).
    Each client runs `driver` process with 32 threads, each thread issuing 10 transactions per seconds. The clients output logs to
    `result_same_s_same_c/exp_8_servers` directory, with the file format `client_<peer_host>_16`.   

2. Running with fixed number of clients and varying number of servers: 
    + Change `LOG_DIR` in `env.sh` to correct location, say `result_fixed_c`
    + Start it (X=16): 

        `. run-bench.sh 16 32 8 10`

    This will start 16 peers (on first 16 nodes in `$HOSTS`) and 8 clients (on 4 first 4 nodes in
    `$CLIENTS`). Each client runs `driver` process with 32 threads. The clients output logs to
    `result_fixed_c/exp_16_servers` directory, with the file format `client_<peer_host>_16`.   


3. Drop off nodes:
    + Change `LOG_DIR` in `env.sh` to correct location, say `result_fixed_c_drop_4`
    + Start it (X=16): 

        `. run-bench.sh 16 32 8 10 -drop`

    This will start 16 peers (on first 16 nodes in `$HOSTS`) and 8 clients (on 4 first 4 nodes in
    `$CLIENTS`). Each client runs `driver` process with 32 threads. At 250th second, 4 peers are killed off,
    while the rest continues to run. The clients output logs to `result_fixed_c_drop_4/exp_16_servers`
      directory, with the file format `client_<peer_host>_16`.   

4. Running with different workloads:

Simply change `$EXE_HOME` and `$BENCHMARK` variables in `env.sh`. Then repeat the steps like in other
examples.  
