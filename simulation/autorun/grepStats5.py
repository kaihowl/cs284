#!/usr/bin/python
import os
import shutil
import sys

def grep_avg_queue(file):
  file.seek(0)
  for line in file:
    if line.find("   1,                                190.0.3.4, [0],     Network,        FIFO,Average Time In Queue")>0:
      return float(line.split()[-1])
      
def grep_polltime(file):
  file.seek(0)
  for line in file:
    if line.find("MAC-802.15.4-POLL-INTERVAL")>0:
      return int(line.split()[4][0:-2])

def grep_workload(file):
  file.seek(0)
  for line in file:
    if line.find("SUPER-APPLICATION")>0:
      return int(line.split()[13])/15

def grep_delay(file, needle):
  delays = []
  file.seek(0)
  for line in file:
    if line.find(needle)>0:
      delays.append(float(line.split()[10]))
  average = sum(delays)/len(delays)
  minDelay = min(delays)
  maxDelay = max(delays)
  return (average, minDelay, maxDelay)
  
        
def grep_req_delay(file):
  return grep_delay(file, "Average End-to-End Delay for request received (s)")
  
def grep_rep_delay(file):
  return grep_delay(file, "Average End-to-End Delay for replies received (s)")

if __name__=="__main__":
  os.chdir("batch/")
  start = int(sys.argv[1])
  end = int(sys.argv[2])
  graph_file = open("graph.tab", "w")
  
  #header
  graph_file.write("poll\tworkload\treq_delay\treq_min\treq_max\trep_delay\trep_min\trep_max\tavg_coord_queue_time[s]\n")
  
  percent = 0
  for i in xrange(start, end):
    if int(i*100/end) > percent:
      percent = int(i*100/end)
      print "Finished %i percent" % percent
    config= open("%i/template.config" % i)
    app = open("%i/template.app" % i)
    stat = open("%i/Experiment-2.stat" % i)
    req_avg, req_min, req_max = grep_req_delay(stat)
    rep_avg, rep_min, rep_max = grep_rep_delay(stat)
    graph_file.write("%i\t%i\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\n" % (grep_polltime(config), grep_workload(app), req_avg, req_min, req_max, rep_avg, rep_min, rep_max, grep_avg_queue(stat)))

    config.close()
    app.close()
    stat.close()
  
  graph_file.close()

