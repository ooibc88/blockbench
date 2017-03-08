
To launch experiments with varying client request rates, we use 

        `python exps.py` 

## Main variables

The `exps.py` scripts contains the following global variables:

1. `NNODES`: a list containing the numbers of nodes

2. `THREADS`: a list containing number of threads per client

3. `RATES`: a list containing number of requests per second per thread. 

4. `EXPS`: list of tuple (#threads,rate) which is preferred to `THREADS` and `RATES` lists when only a small
set from `THREADS.RATES` list is being considered. 

## Other settings

By default, the experiments use YCSB workload. Variables like binary path, log path, etc. are found in
'env_ycsb.sh'. 


## Start the benchmark

To run it, specify the values for configuration variables, then run:

        `python exps.py`. 
        
For every number of nodes in `NNODES`, the script iterates through all combination of (#threads, rate) and invoke:

        `run_bench.sh <#nodes> <#threads> <#clients> <#req rate>`


