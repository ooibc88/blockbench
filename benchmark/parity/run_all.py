#!/bin/python
import os
import time
# re-run the saturation and security benchmark

cp_cmd='cp config_{}.py config.py'
run_cmd='python run.py start {}'
def run_benchmark(name, param):
  # copy the config file
  os.system(cp_cmd.format(name))
  os.system(run_cmd.format(param))

if __name__=='__main__':
  run_benchmark('saturation', '')
  time.sleep(10)
  run_benchmark('security', '-security')
  time.sleep(10)
  run_benchmark('scale', '')
  time.sleep(10)
  run_benchmark('scale_fix_8', '-fixed')
  time.sleep(10)
  run_benchmark('drop', '-drop')
  time.sleep(10)
  run_benchmark('smallbank', '-smallbank')
  time.sleep(10)
  run_benchmark('donothing', '-donothing')
  time.sleep(10)
  print 'DONE ALL BENCHMARKS for Parity'
