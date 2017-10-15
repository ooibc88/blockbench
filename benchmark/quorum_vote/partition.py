#!/bin/python
import os
import sys
from config import *

def partition(nodes, timeout):
  n=len(nodes)
  nodes1 = nodes[:n/2]
  nodes2 = nodes[n/2:]
  for n1 in nodes1:
    for n2 in nodes2:
      cmd = partition_cmd.format(n1,n2,timeout)
      os.system(cmd)

if __name__=='__main__':
  partition(NODES[:int(sys.argv[1])], TIMEOUT)
