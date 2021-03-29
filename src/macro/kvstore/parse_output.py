#! /usr/bin/env python

import sys
import re


def main():
    if len(sys.argv) != 2 or sys.argv[1] == '-h':
        print("Usage: %s OutputFileName" % sys.argv[0])
        print("Statistics (.cv file) for each workload " + \
              "will be written to the workload directory.")
        sys.exit(-1)

    path = sys.argv[1]

    with open(path) as file_in:
        with open("workloads/workloada.csv", "w") as f:
            f.write("txt_count,latency,outstanding\n")
            for line in file_in:
                txt_count = re.search('tx count = (.+?) ', line)
                latency = re.search('latency = (.+?) ', line)
                outstanding_request = re.search('outstanding request = (.*)', line)
                if (latency and txt_count and outstanding_request):
                    txt_count_res = txt_count.group(1)
                    latency_res = latency.group(1)
                    outstanding_request_res = outstanding_request.group(1)
                    f.write(txt_count_res + "," + latency_res + "," + outstanding_request_res + "\n")


if __name__ == '__main__':
    main()
