import sys

fd1 = open ('graph_90.tab', 'r')
fd2 = open ('graph_180.tab', 'r')
fd3 = open ('graph_295.tab', 'r')

#print sys.argv[1]
for line1 in fd1:
	line2 = fd2.readline()
	line3 = fd3.readline()

	myline1 = line1.split()
	myline2 = line2.split()
	myline3 = line3.split()
	#print myline2[1]
	print str(float(myline1[0])/1000.0) + "\t" +myline1[1] + "\t" + myline2[1] + "\t" + myline3[1];

