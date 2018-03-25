#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Feb 21 19:32:43 2018
Solve pendulum motion without small angle approximation
Using RK2 method

@author: Apiwat Wisitsorasak
"""

import numpy as np
import matplotlib.pyplot as plt

# Inputs, all units are in SI
y0 = 30*np.pi/180       # Initial angle
w0 = 0                  # Initial angular speed
g = 9.8                 # Gravitational acceleration
l = 1.0                 # Length of the pendulum
m = 1.0                 # Mass of the pendulum
b = 1.0                 # f0/m/l
wd = 3.2                # Driven anguular frequency

def fy(omega): # dy/dt = omega
    return omega

def fw(theta,t): # d omega/dt = F/l/m
    global g, l, wd, b
    return -g*np.sin(theta)/l + b*np.cos(wd*t)

# Main computation
w = w0
y = y0
t = 0
# Save data to lists used for visualization
ww = [w]
yy = [y]
tt = [t]

dt = 0.0001
NStep = 400000
i = 1

while i<=NStep:
    ky1 = dt*fy(w)
    kw1 = dt*fw(y,t)
    
    ky2 = dt*fy(w+0.5*kw1)
    kw2 = dt*fw(y+0.5*ky1,t+0.5*dt)
    
    y = y + ky2
    w = w + kw2
    
    t = t + dt
    
    yy.append(y)
    ww.append(w)
    tt.append(t)
    
    i = i+1

# Visualization
plt.subplot(211)
plt.plot(tt,yy,'r-')
plt.xlabel('time')
plt.ylabel(r'$\theta(t)$ [radians]')
plt.subplot(212)
plt.plot(yy,ww,'b-')
plt.xlabel(r'$\theta$')
plt.ylabel(r'$\omega$')
plt.show()
