#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Feb 27 09:16:55 2018
Solve temperature field of a nuclear waste rod by using Euler method

@author: Apiwat Wisitsorasak
"""
import numpy as np
import matplotlib.pylab as plt
# Parameters (All variables are in SI units)
c = 789.0           # Heat capacity
sigma = 1.00        # Emissivity ? [W/mK]
rho = 2.00e3        # Density
kappa = sigma/c/rho
a = 0.25
xmin = 0.0
xmax = 100.0e-2
T0 = 1.0
tau0 = 100.0
TE = 300

NGrid = 100
NTime = 1000
dx = (xmax - xmin)/NGrid
dt = 100

Ta = np.ones((1,NGrid))*TE
Tb = np.zeros((1,NGrid))

ti = dt
tn = 1
while tn<=NTime:
    xi = 1
    while xi<=NGrid:
        if xi == 1:
            A = 0.0
            B = 1.0 - 2.0*kappa*dt/dx/dx + kappa*dt/dx/dx
            C = kappa*dt/dx/dx
            S = T0/a/a*np.exp(-ti/tau0)
            Tb[0,xi-1] = B*Ta[0,xi-1] + C*Ta[0,xi-1+1] + kappa*dt*S
        elif xi == NGrid:
            A = kappa*dt/dx/dx
            B = 1.0 - 2.0*kappa*dt/dx/dx
            C = 0.0
            S = 0.0
            Tb[0,xi-1] = A*Ta[0,xi-1-1] + B*Ta[0,xi-1] + (kappa*dt/dx/dx)*TE
        else:
            A = kappa*dt/dx/dx
            B = 1.0 - 2.0*kappa*dt/dx/dx
            C = kappa*dt/dx/dx
            S = 0.0
            Tb[0,xi-1] = A*Ta[0,xi-1-1] + B*Ta[0,xi-1] + C*Ta[0,xi-1+1] + kappa*dt*S
        xi = xi + 1
    Ta = Tb
    Tdiff = Ta - np.ones((1,NGrid))*TE
    print(Ta[0,0:5:1])
    ti = ti + dt
    tn = tn + 1

XX = np.zeros((1,NGrid))
i=1
while i<=NGrid:
    XX[0,i-1] = i*dx
    i = i + 1
plt.plot(XX,Tdiff,'o')

