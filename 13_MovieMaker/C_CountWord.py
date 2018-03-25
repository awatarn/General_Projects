#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Dec 12 22:25:08 2017

@author: vitreloy
"""

filename = "Tf2.txt"
file1 = open(filename,"r")

TotalTimeSteps = 0

for line in file1:
    if "-99" in line:
        TotalTimeSteps += 1
        
print("Total number of time steps in",filename,"is ",TotalTimeSteps,"steps")