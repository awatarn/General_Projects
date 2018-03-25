#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Mar 23 11:06:46 2018

Purpose: Visualize the analytical solutions to the GS equation
Ref: Phys. Plasmas 17, 032502 (2010)

@author: Apiwat Wisitsorasak
"""
import numpy as np
import matplotlib.pyplot as plt

# Input parameters
epsilon = 0.32
kappa = 1.7
delta = 0.33
B0 = 5.3
Ip = 15
R0 = 6.2

alpha = np.arcsin(delta)

def xx(tau):
    global epsilon, alpha
    return 1.0 + epsilon*np.cos(tau + alpha * np.sin(tau))

def yy(tau):
    global epsilon, kappa
    return epsilon*kappa*np.sin(tau)

tt = np.arange(0,2*np.pi,2*np.pi/1000.0)
xi = [xx(0)]
yi = [yy(0)]

i=1
while i<tt.size:
    xi.append(xx(tt[i]))
    yi.append(yy(tt[i]))
    i = i + 1

plt.plot(xi,yi)

