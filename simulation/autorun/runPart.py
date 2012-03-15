#!/usr/bin/python
import sys
import shutil
import os
from datetime import datetime

def runSingleExp(config_file, number):
  print "----------------> Running Experiment %i" % number
  start = datetime.now()
  os.chdir(str(number))
  os.system("qualnet %s" % config_file) 
  os.chdir("..")
  stop = datetime.now()
  return stop-start

def runExp(config_file, start, end):
  for i in xrange(start, end):    
    duration = runSingleExp(config_file, i)
    exp_left = end-(i+1)
    endtime = duration*exp_left+datetime.now()
    print "-----------------> ETA:%s <--------------------" % str(endtime)

def usage():
  print "Usage:\n"
  print "{0} configfilename start end" % sys.argv[0]

if __name__=="__main__":
  if len(sys.argv) == 1:
    usage()
    sys.exit()
  os.chdir("batch")
  start = int(sys.argv[2])
  end = int(sys.argv[3])
  config_file = str(sys.argv[1])
  runExp(config_file, start, end)
