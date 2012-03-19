import sys

fd = open ('graph.tab', 'r')

#print sys.argv[1]
for line in fd:
	myline =  line.split()
	if (myline[0]==sys.argv[1]):
		print str(float(myline[1])/60.0) + "\t" + str(myline[2])

