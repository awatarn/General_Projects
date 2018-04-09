#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sun Apr  8 20:15:39 2018

@author: Apiwat Wisitsorasak

This program reads a 1D simulation data from TASK/TR output and plot it.
The graph is also export as a EPS figure.
"""
import matplotlib.pyplot as plt
import math

def PlotGraph1D(filename1, XLabel, YLabel, Title, FigureName):
    # filename1 = 'data_GT0.txt'
    # XLabel = 'Time [s]'
    # YLabel = 'NE [10**20 m**-3]'
    # Title  = 'Electron density' 
    # FigureName = 'Plot1.eps'
    
    # OPEN FILE
    file1 = open(filename1,"r")
    
    # IGNORE THE FIRST TWO LINES
    temp1 = file1.readline() # line 1
    temp1 = file1.readline() # line 2
    
    temp1 = file1.readline() # line 3
    NData = int(temp1[5:])   # Total number of data points
    
    XList = []
    YList = []
    
    Linei = 1
    while Linei<=NData:
        temp1 = file1.readline()
        temp2 = temp1.split(" ")
        i = -1
        for itemi in temp2:
            if (len(itemi)>2):
                i = i*(-1)
                if (i>0):
                    xi = float(itemi)
                    XList.append(xi)
                else:
                    yi = float(itemi)
                    YList.append(yi)                    
        Linei = Linei + 1
    
    # GRAPH PLOTING
    plt.rc('font', family='serif', serif='Times')
    
    width = 5.5
    height = 4.5
    
    fig = plt.figure(figsize=(width, height))
    
    plt.plot(XList, YList, 'k-', linewidth=2)
    plt.xlabel(XLabel)
    plt.ylabel(YLabel)
    plt.grid()
    plt.xlim((math.floor(min(XList)), max(XList)))
    plt.title(Title)
    
    fig.set_size_inches(width, height)
    fig.savefig(FigureName)
    
# MAIN PROGRAM
filename1 = 'data_GT0.txt'
XLabel = 'Time [s]'
YLabel = 'NE [10**20 m**-3]'
Title  = 'Electron density' 
FigureName = 'Plot1.eps'
PlotGraph1D(filename1, XLabel, YLabel, Title, FigureName)

