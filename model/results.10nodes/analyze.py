#!/usr/bin/env python

import sys
from numpy import poly1d

if len(sys.argv) < 2:
	print "USAGE: "+sys.argv[0]+" [workload]"
	exit()

workload = sys.argv[1]
filename = 'graph_'+workload+'.tab'
fd = open (filename, 'r')

for line in fd:
	myline =  line.split()
	sleep = float(myline[0])
	wait  = float(myline[1])
	myworkload = int(workload)/60.0
	term = wait - (sleep/2)
	P = poly1d([ 1, term, -1*term/myworkload])
	print str(sleep)+"\t"+str(P.r[1])


