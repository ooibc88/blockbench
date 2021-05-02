#!/bin/python

import os
import time
import sys

# running experiments
EXPS = [(1, 20)]
TARGET = "ethereum"
WORKLOAD = "workloada.spec"
ENDPOINT = "localhost:8545"
SC = "ycsb"
WAIT_TIME = 20
IS_INT = 1
OUTPUT_FILE = "output.txt"


def run_exp():
    cmd = './driver -db {} -threads {} -P workloads/{} -txrate {} -endpoint {} -wl {} -wt {} -isint {} 2>&1 | tee {}'

    # for t in THREADS:
    #  for r in RATES:
    for (t, r) in EXPS:
        # run the experiments
        print("#### ####")
        print(cmd.format(TARGET, t, WORKLOAD, r, ENDPOINT, SC, WAIT_TIME, IS_INT, OUTPUT_FILE))
        print("#### ####")
        os.system(cmd.format(TARGET, t, WORKLOAD, r, ENDPOINT, SC, WAIT_TIME, IS_INT, OUTPUT_FILE))



if __name__ == '__main__':
   start = time.time()
   run_exp()
   interval = time.time() - start
   print(2000/interval)
   print(interval)
   print("done")
