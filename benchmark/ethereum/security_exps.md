# How to run the security experiments

There are a number of Python and bash scripts needed to run the security experiments. 

1. `config.py` contains several important global variables to be imported to other Python's scripts:
      + `NODES`: the list of strings representing the server IP addresses.
      + `partition_cmd`: use to specify the script that simulates partition attacks. 
      + `TIMEOUT`: how long the partition lasts 

2. `partition.py`: implements the function `partition(node_list, time_out)` that partitions the nodes in the
given `node_list` in half, and keeps the partition for `time_out` seconds. It basically invokes the following
scripts with different parameters:

        ` partition.sh <node_i> <node_j> <time_out>`

3. `partition.sh <node_i> <node_j> <time_out>`: drops TCP connection between the 2 nodes for the given
`time_out` seconds. 

4. `run-bench-security.sh <#nodes> <#threads> <#clients> <#req rate>`: works the same way as `run-bench.sh`,
except that it invokes `partition.py` at 100th second. 

4. `exps.py`: contains the main script for launching the experiments:

        ` python exps.py -security`

