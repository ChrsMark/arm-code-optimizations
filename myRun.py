#!/usr/bin/env python
import re
import os

#into this list add all the x-values
Bxs = [2,3,4,8,9,12,16,18,23,36,46,72,144]
#into this list add all the y-values
Bys = [2,4,8,11,16,22,44,88,176]

love = open("love.txt", "w")  #here we open the file to write in it later
results = open("results.txt", "w")
for x in Bxs:
	for y in Bys:
		print "Running for dimension "+str(x)+"x"+str(y)+" now\n"
		counter=0
		for time in range(30):
			os.system('./phodsOPT '+str(x)+' '+ str(y) +' | tee love.txt')  
			love.close()
			love = open("love.txt", "r")
			
			for line in love:
				token = line.split()
				counter += float(token[0])
			#print "Counter is :"+str(counter)
		myResult = counter/30.0 
		print myResult
		print >>results,str(x)+"\t" +str(y)+"\t" + str(myResult)
	print >>results,""
