# -*- coding: utf-8 -*-
"""
Created on Wed Sep 06 10:10:33 2017

@author: rsegovia
"""

infile = open("./fps.txt","r+")
outfile = open("./fpsdot.txt","w")
for line in infile:
    line = line.replace(".",",")
    outfile.write(line)
infile.close()
outfile.close()
    