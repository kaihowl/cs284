import sys

fd = open ('graph.tab', 'r')

#print sys.argv[1]
for line in fd:
	myline =  line.split()
	if (myline[1]==sys.argv[1]):
		print myline[0] + "\t" +(str(float(myline[2])+float(myline[5])))

