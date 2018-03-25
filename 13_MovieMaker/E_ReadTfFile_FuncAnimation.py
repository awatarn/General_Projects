#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Dec 11 22:15:22 2017

https://matplotlib.org/examples/animation/dynamic_image2.html
https://matplotlib.org/examples/animation/dynamic_image.html
http://www.peterbeerli.com/classes/images/2/26/Isc4304matplotlib6.pdf
https://stackoverflow.com/questions/24166236/add-text-to-image-animated-with-matplotlib-artistanimation

@author: Apiwat Wisitsorasak
"""
import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import matplotlib.animation as animation

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

# 2 Load all data from files to internal variables
TDataXY0 = []
TimeStepCounter = 0 # Time step counter, starting from 1
while temp1!='':

    temp1 = file1.readline()
    
    if "-99" in temp1:
#        print("Find -99")
        TimeStepCounter = TimeStepCounter + 1
#        print(TimeStepCounter)
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
        
        DataXY0 = np.zeros((yy,xx))
        rowCounter = 0
        while rowCounter<yy:
            temp1 = file1.readline()
            temp2 = temp1.split(" ")
            del temp2[-1] # remove the last element which contains "\n"
            rowi = [float(i) for i in temp2]
            
            DataXY0[rowCounter] = rowi
            rowCounter = rowCounter + 1
        
        # Append the data to a list
        TDataXY0.append(DataXY0)
        
# 3 Create Matplot animation
print('Create animation')

TotalMovieFrames = 200
       
fig, (ax0, ax1) = plt.subplots(2,1)

ims = []

for i in range(TotalMovieFrames):
    if i==0:
        im0 = ax0.imshow(TDataXY0[i], clim=(290, 329), cmap="jet", origin='lower', animated=True)
        fig.colorbar(im0, ax=ax0)
        im1 = ax1.imshow(TDataXY0[i], clim=(290, 329), cmap="jet", origin='lower', animated=True)        
        fig.colorbar(im1, ax=ax1)
    else:
        im0.set_array(TDataXY0[i])
        im1.set_array(TDataXY0[i])
    temp0 = 'Fictive temperature i = ' + str(i);
    print(temp0)
    text0 = ax0.text(5, 22, temp0)
    temp1 = 'Xxx i = ' + str(i);
    print(temp1)
    text1 = ax1.text(5, 22, temp1)
    ims.append([im0,im1,text0,text1])
    
ani = animation.ArtistAnimation(fig, ims, interval=25, blit=True)

ani.save('MovieTest.mp4',fps=8)

plt.show()

#plt.close(fig)



        


    


