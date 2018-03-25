#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Dec 11 22:15:22 2017

@author: Apiwat Wisitsorasak
"""
import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import matplotlib.animation as manimation

# 0 Count the number of time steps in a file
filename = "Tf2.txt"
file0 = open(filename,"r")

TotalTimeSteps = 0

for line in file0:
    if "-99" in line:
        TotalTimeSteps += 1
        
print("Total number of time steps in",filename,"is ",TotalTimeSteps,"steps")

# 1 Open this file again for create a movie file
file1 = open("Tf2.txt","r")
temp1 = file1.readline()
print(temp1) # first line, it contains 1, do nothing
temp1 = file1.readline()
print(temp1) # second line, dimensions, X Y
xx,yy = temp1.split(" ")
xx = int(xx) # number of columns
yy = int(yy) # number of rows

# 1.2 Set parameters for a movie file
FFMpegWriter = manimation.writers['ffmpeg']
metadata = dict(title='Movie Test', artist='Matplotlib',
                comment='Movie support!')
writer = FFMpegWriter(fps=15, metadata=metadata)

matplotlib.rcParams.update({'font.size': 30})
fig = plt.figure(figsize=(16,12))


TotalMovieFrames = 40
with writer.saving(fig, "TestMovieImshow.mp4", TotalMovieFrames):
    
    TimeStepCounter = 0 # Time step counter, starting from 1
    while temp1!='':
    
        temp1 = file1.readline()
        
        if "-99" in temp1:
    #        print("Find -99")
            TimeStepCounter = TimeStepCounter + 1
            print(TimeStepCounter)
            var99,timei,Tambient,Ntimei,dti = temp1.split(" ")
            timei=float(timei) # actual time
            Tambient=float(Tambient) # ambient temperature
            Ntimei = int(Ntimei) # the ith time step
            dti = float(dti) # time-step difference
            
            if TimeStepCounter == 1: #
                Data1DTime = np.array([[TimeStepCounter,Tambient,Ntimei,dti]])
            else:
                newrow = np.array([[TimeStepCounter,Tambient,Ntimei,dti]])
                Data1DTime = np.vstack([Data1DTime,newrow])
            
            DataXY = np.zeros((yy,xx))
            rowCounter = 0
            while rowCounter<yy:
                temp1 = file1.readline()
                temp2 = temp1.split(" ")
                del temp2[-1] # remove the last element which contains "\n"
                rowi = [float(i) for i in temp2]
                
                DataXY[rowCounter] = rowi
                rowCounter = rowCounter + 1
            
            plt.imshow(DataXY, clim=(290, 329), cmap="jet")
            if TimeStepCounter == 1:
                plt.colorbar()
            writer.grab_frame()
            
            if TotalMovieFrames == TimeStepCounter:
                break
    #        plt.imshow(DataXY, clim=(290, 329), cmap="jet")
    #        plt.colorbar()

    


