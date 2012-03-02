#!/usr/bin/python
import sys
import shutil
import os

def runSingleExp(number):
  print "----------------> Running Experiment %i" % number
  os.chdir(str(number))
  os.system("qualnet template.config")
  os.chdir("..")

def runExp(start, end):
  for i in xrange(start, end):
    runSingleExp(i)

if __name__=="__main__":
  os.chdir("batch")
  start = int(sys.argv[1])
  end = int(sys.argv[2])
  runExp(start, end)
