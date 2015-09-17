#!/usr/bin/env python
import re
import os

#into this list add all the x-values
Bxs = 16
#into this list add all the y-values
Bys = 16

love = open("love.txt", "w")  #here we open the file to write in it later
results = open("res_1_1.txt", "w")
x = Bxs
y = Bys
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

print >>results,str(x)+"\t" +str(y)+"\t" + str(myResult)
