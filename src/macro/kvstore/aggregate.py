#!/usr/bin/env python
import sys, os

filenum = int(sys.argv[1])
linenum = 0

with open(sys.argv[2]) as f:
    lines = f.read().splitlines()
    for line in lines:
        if line.find('latency') != -1:
            linenum += 1

print "line number: ", linenum

latency_arr = [float(0)]*linenum
throughput_arr = [float(0)]*linenum

for i in range(0, filenum):
    print 2+i, sys.argv[2+i]
    with open(sys.argv[2+i]) as f:
        lines = f.read().splitlines()
    j = 0
    for line in lines:
        if line.find('latency') != -1:
            latency = float(line.split(' ')[-2])
            latency_arr[j] += latency

        elif line.find('Throughput') != -1:
            tp = float(line.split(' ')[-1])
            throughput_arr[j] += tp
            j = j+1

for la, tp in zip(latency_arr, throughput_arr):
    print la/filenum, tp
