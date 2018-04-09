#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Apr  8 08:16:44 2018

@author: Apiwat Wisitsorasak

This program read 2D data from TASK/TR code and convert it into a new format
which is easier for reading by java script
Line 1 to 5 are header part.
Line 6 and the rest are snapshots of the radial profiles of the plasma.
"""
import math

def Convert2DFileFormat(filename1, filename2):
    # INPUT FILE
    # filename1 = "data_RT5.txt"
    # OUTPUT FILE
    # filename2 = "data2D_RT5.txt"
    
    file1 = open(filename1,"r")
    file2 = open(filename2,"w")
    
    temp1 = file1.readline()
    file2.write(temp1)
    var1 = temp1[5:7]
    
    temp1 = file1.readline()
    temp1 = file1.readline()
    NRMax = int(temp1[5:13])
    NTMax = int(temp1[13:21])
    file2.write("NRMax = {0:d}\n".format(NRMax))
    file2.write("NTMax = {0:d}\n".format(NTMax))
    
    # GET RADIAL COORDINATES
    linei = 1
    elementn = 0
    RList = []
    RLineTotal = math.ceil(NRMax/5.0)
    while linei<=RLineTotal:
        temp1 = file1.readline()
        temp1 = temp1[:-1]
        temp2 = temp1.split(" ")    
        for i in temp2:
            if(len(i)>=2):
                elementi=float(i)
                elementn = elementn + 1
                RList.append(elementi)
        linei = linei + 1
    # SAVE RADIAL COORDINATES TO FILE2    
    for ri in RList:
        file2.write(" {0:14.7e}".format(ri))
    file2.write("\n")
    
    # GET TIME LIST
    linei = 1
    elementn = 0
    TList = []
    TLineTotal = math.ceil(NTMax/5.0)
    while linei<=TLineTotal:
        temp1 = file1.readline()
        temp1 = temp1[:-1]
        temp2 = temp1.split(" ")    
        for i in temp2:
            if(len(i)>=2):
                elementi=float(i)
                elementn = elementn + 1
                TList.append(elementi)
        linei = linei + 1
    # SAVE TIME LIST TO FILE
    for ti in TList:
        file2.write(" {0:14.7e}".format(ti))
    file2.write("\n")
    
    # SAVE RADIAL PROFILE AT TIME-I IN ROW-I
    TimeSectioni = 1
    while TimeSectioni<=NTMax:
        linei = 1
        while linei <= RLineTotal:
            temp1 = file1.readline()
            temp1 = temp1[:-1]
            temp2 = temp1.split(" ")
            for i in temp2:
                if(len(i)>=2):
                    elementi = float(i)
                    file2.write(" {0:14.7e}".format(elementi))        
            linei = linei + 1
        file2.write("\n")
        TimeSectioni = TimeSectioni + 1
    
    file2.close()

# MAIN PROGRAMME
# INPUT FILE
filename1 = "data_RT1.txt"
# OUTPUT FILE
filename2 = "data2D_RT1.txt"

Convert2DFileFormat(filename1, filename2)