# Yahoo! Cloud System Benchmark
# Workload A: Update heavy workload
#   Application example: Session store recording recent actions
#                        
#   Read/update ratio: 50/50
#   Default data size: 1 KB records (10 fields, 100 bytes each, plus key)
#   Request distribution: zipfian

recordcount=2000
operationcount=10
workload=ycsb

readallfields=true
fieldcount=1

readproportion=0
updateproportion=1
scanproportion=0
insertproportion=0

requestdistribution=uniform

