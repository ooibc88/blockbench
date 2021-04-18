import os
import sys
import getopt
import re
import time
from datetime import datetime

use_cpu_utilization = True

url = "http://localhost:8086/status"
min_containers = 1
max_containers = 6
peer = "peer"
sealer = "sealer"
backup = "backup"
application = "application"
check_interval = 5
up_threshold = 85 if use_cpu_utilization else 20
down_threshold = 60 if use_cpu_utilization else -1
log_cpu_path = "ethereum.csv"


def printSetting():

    print("Min containers:  \t%d" % min_containers)
    print("Max containers:  \t%d" % max_containers)
    print("Check interval:  \t%d seconds" % check_interval)
    if use_cpu_utilization:
        print("Up threshold:    \t> %.2f%% cpu utilization" % up_threshold)
        print("Down threshold:  \t< %.2f%% cpu utilization" % down_threshold)
    else:
        print("Up threshold:    \t> %d waiting requests" % int(up_threshold))
        print("Down threshold:  \t< %d waiting requests" % int(down_threshold))


def printUsage():
    print(
        """
        Usage: %s [options]
            -h or --help: show help info
            -l url or --link=url: the status url of nginx
            -m min_containers or --min=min_containers: the min number of containers
            -M max_containers or --max=max_containers: the max number of containers
            -t target_container or --target=target_container: the target container
            -i check_interval or --interval=check_interval: the checking interval
            -u up_threshold or --up=up_threshold: the threshold for scaling up
            -d down_threshold or --down = down_threshold: the threshold for scaling down
        """
        % (sys.argv[0],))


def check_cpu_utilization(log_file):
    pattern = re.compile(r".*%s.*" % peer)
    pattern1 = re.compile(r".*%s.*" % sealer)
    pattern2 = re.compile(r".*%s.*" % application)
    pattern3 = re.compile(r".*%s.*" % backup)
    cpus = []
    mems = []
    with os.popen("sudo docker stats --no-stream") as f:
        for s in f.readlines():
            ss = s.split()
            if len(ss) >= 3 and (pattern.match(ss[1]) or pattern1.match(ss[1])
                                 or pattern2.match(ss[1]) or pattern3.match(ss[1])):
                cu = float(ss[2].replace("%", ""))
                cpus.append(cu)
                name = ss[1]
                mem = float(ss[6].replace("%", ""))
                mems.append(mem)
                print("INFO: container %s: cpu %.2f%%, mem %.2f%%" % (ss[1], cu, mem))
    num = len(cpus)
    avg_cpu = sum(cpus)  / num if num > 0 else -1
    avg_mem = sum(mems)  / num if num > 0 else -1
    log_file.write("%s,%d,%.2f,%.2f,%s\n" % (datetime.now().strftime("%H:%M:%S"),
                                             num, avg_cpu, avg_mem,
                                             ",".join("%.2f,%.2f" % (cpus[i], mems[i]) for i in range(num))))
    log_file.flush()
    return avg_cpu



try:
    opts, args = getopt.getopt(sys.argv[1:],
                               "hl:m:M:t:i:u:d:",
                               ["help", "link=", "min=", "max=", "target=", "interval=", "up=", "down="]
                               )
except getopt.GetoptError:
    print("Error: Invalid arguments!")
    sys.exit(-1)

for cmd, arg in opts:
    if cmd in ("-h", "--help"):
        printSetting()
        print("")
        printUsage()
        sys.exit(0)
    elif cmd in ("-l", "--link"):
        url = arg
    elif cmd in ("-m", "--min"):
        min_containers = max(1, int(arg))
    elif cmd in ("-M", "--max"):
        max_containers = int(arg)
    elif cmd in ("-t", "--target"):
        target_container = target_container
    elif cmd in ("-u", "--up"):
        up_threshold = float(arg)
    elif cmd in ("-d", "--down"):
        down_threshold = float(arg)
    elif cmd in ("-i", "--interval"):
        check_interval = int(arg)

printSetting()
print("")

log_file = open(log_cpu_path, "w+")
log_file.write("Time,Num,AvgCPU,AvgMEM,C1CPU,CIMEM,...\n")

while True:
    start_time = time.time()

    print("INFO:\tStart checking ...")

    if use_cpu_utilization:
        avg_cu = check_cpu_utilization(log_file)
        print("avg cpu .. ", avg_cu)
        end_time = time.time()
    sleep_time = check_interval - (end_time - start_time)

    print("INFO:\tFinish checking. Sleeping %.2f seconds ...\n" % sleep_time)
    if sleep_time > 0:
        time.sleep(sleep_time)

log_file.close()
