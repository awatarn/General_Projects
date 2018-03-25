#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Dec 14 22:00:08 2017
This program analyzes ip addresses which attemped to ssh to DellServer.
IP addresses and counts are reported.
@author: vitreloy
"""
import re
import collections

filename = "A_SshdFailed.txt"  # IP addresses that attemped to ssh but failed.
file0 = open(filename,"r")


TotalLines = 0

pattern1 = 'from ' # On each line, ip number is in between 'from' and 'end'.
pattern2 = 'port'

if 0:
    for line in file0:
        if "fusion sshd" in line:
            TotalLines += 1
            s = re.search(pattern1, line)
            pos1 = s.end() # first index of the ip number
            
            e = re.search(pattern2, line)
            pos2 = e.start()-1 # last index of the ip number
            
            print(line[pos1:pos2])
    #        if TotalLines>10:
    #            break
        
        
#print("Total number of lines in ",filename,"is ",TotalLines,"lines")


if 1:
    with open('A_ListIPNumbers.txt') as infile:
        counts = collections.Counter(l.strip() for l in infile)
    for line, count in counts.most_common():
        print(count) # print the number of attemps
#        print(line, count) # IP and Count






