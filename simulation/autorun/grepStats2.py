#!/usr/bin/python
import os
import shutil
import sys

def grep_polltime(file):
  for line in file:
    if line.find("MAC-802.15.4-POLL-INTERVAL")>0:
      return int(line.split()[4][0:-2])

def grep_workload(file):
  for line in file:
    if line.find("SUPER-APPLICATION")>0:
      return int(line.split()[13])/15
      
def grep_delay(file):
  delays = []
  for line in file:
    if line.find("Average End-to-End Delay for request received (s)")>0:
      delays.append(float(line.split()[10]))
  average = sum(delays)/len(delays)
  minDelay = min(delays)
  maxDelay = max(delays)
  return (average, minDelay, maxDelay)

if __name__=="__main__":
  os.chdir("batch/")
  start = int(sys.argv[1])
  end = int(sys.argv[2])
  graph1_file = open("graph1.tab", "w")
  graph2_file = open("graph2.tab", "w")
  graph3_file = open("graph3.tab", "w")
  graph4_file = open("graph4.tab", "w")
  
  #header
  for file in [graph1_file, graph2_file, graph3_file]:
    file.write("poll\tdelay\tmin\tmax\n")
  
  for i in xrange(start, end):
    config= open("%i/template.config" % i)
    app = open("%i/template.app" % i)
    stat = open("%i/Experiment-2.stat" % i)
    # print "poll: %i workload: %i delaysStat: %s" % (grep_polltime(config), grep_workload(app), str(grep_delay(stat)))
    sleep = grep_polltime(config)
    if sleep == 20:
      outfile = graph1_file
    elif sleep == 100:
      outfile = graph2_file
    elif sleep == 500:
      outfile = graph3_file
    elif sleep == 1000:
      outfile = graph4_file
    else:
      sys.exit("Unkown workloads")
    
    avg, minDel, maxDel = grep_delay(stat)
    outfile.write("%i\t%.3f\t%.3f\t%.3f\n" % (grep_workload(app), avg, minDel, maxDel))

    config.close()
    app.close()
    stat.close()
  
  graph1_file.close()
  graph2_file.close()
  graph3_file.close()
  graph4_file.close()

