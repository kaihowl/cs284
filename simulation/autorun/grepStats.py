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
      
if __name__=="__main__":
  os.chdir("batch/")
  start = int(sys.argv[1])
  end = int(sys.argv[2])
  for i in xrange(start, end):
    config= open("%i/template.config" % i)
    app = open("%i/template.app" % i)
    print "poll: %i workload: %i" % (grep_polltime(config), grep_workload(app))
  

  