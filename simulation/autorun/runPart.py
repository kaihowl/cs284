#!/usr/bin/python
import sys
import shutil
import os
from datetime import datetime

def runSingleExp(number):
  print "----------------> Running Experiment %i" % number
  start = datetime.now()
  os.chdir(str(number))
  os.system("qualnet template.config")
  os.chdir("..")
  stop = datetime.now()
  return stop-start

def runExp(start, end):
  for i in xrange(start, end):    
    duration = runSingleExp(i)
    exp_left = end-(i+1)
    endtime = duration*exp_left+datetime.now()
    print "-----------------> ETA:%s <--------------------" % str(endtime)

if __name__=="__main__":
  os.chdir("batch")
  start = int(sys.argv[1])
  end = int(sys.argv[2])
  runExp(start, end)
