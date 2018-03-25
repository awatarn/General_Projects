#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Feb 27 19:56:23 2018

@author: vitreloy
"""

#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Tue Feb 27 09:16:55 2018
Solve temperature field of a nuclear waste rod by using Euler method

@author: Apiwat Wisitsorasak
"""
import numpy as np
import matplotlib.pylab as plt

c = 789.0
sigma = 1.00
rho = 2.00e3
#kappa = sigma/c/rho   # SI unit
kappa = 2.0e7 # m**2/100 yr

a = 25
xmin = 0.0
xmax = 100.0
T0 = 1.0
tau0 = 1.0
TE = 300

NGrid = 100
dx = (xmax - xmin)/NGrid
NTime = 1000
dt = 1.0e-3

Ta = np.ones((NGrid,1))*TE
Tb = np.zeros((NGrid,1))
AA = np.zeros((NGrid,NGrid))
BB = np.zeros((NGrid,1))

Tdiff = Ta - np.ones([NGrid,1])*TE
TD = Tdiff.T

ti = dt
tn = 1
while tn<=NTime:    
    xi = 1
    while xi<=NGrid:
        i = xi-1
        gi = kappa*dt/((xi*dx)*dx*dx)
        if (xi*dx)<=a:
            S = (T0/a/a)*np.exp(-ti/tau0)
        else:
            S = 0.0
        if xi == 1:
            A = 0.0
            B = 1.0 + gi*((xi+0.5)*dx) + gi*((xi-0.5)*dx) - gi*((xi-0.5)*dx)
            C = -gi*((xi+0.5)*dx)
            D = Ta[i] + kappa*dt*S
            AA[i][i]   = B
            AA[i][i+1] = C
            BB[i]      = D            
        elif xi == NGrid:
            A = -gi*((xi-0.5)*dx)
            B = 1.0 + gi*((xi+0.5)*dx) + gi*((xi-0.5)*dx)
            C = 0.0
            D = Ta[i] + kappa*dt*S + gi*((xi+0.5)*dx)*TE
            AA[i][i-1] = A
            AA[i][i]   = B
            BB[i]      = D            
        else:
            A = -gi*((xi-0.5)*dx)
            B = 1 + gi*((xi+0.5)*dx) + gi*((xi-0.5)*dx)
            C = -gi*((xi+0.5)*dx)
            D = Ta[i] + kappa*dt*S
            AA[i][i-1] = A
            AA[i][i]   = B
            AA[i][i+1] = C
            BB[i]      = D
        xi = xi + 1
    Tb = np.dot(np.linalg.inv(AA),BB)
    Ta = Tb
    Tdiff = Ta - np.ones([NGrid,1])*TE
    TD = np.append(TD,Tdiff.T,axis=0)
    ti = ti + dt
    tn = tn + 1

plt.figure(1)
plt.imshow(TD)

i = 1
R = np.ones(NGrid)
while i<=NGrid:
    R[i-1] = i*dx
    i = i + 1
    
plt.figure(2)    
plt.plot(R,TD[0],'r--',R,TD[10],'go',R,TD[50],'bs',R,TD[500],'k*',R,TD[1000],'m.')
plt.legend(('0','1 year','5 years','50 years','100 years'))
plt.xlabel(r'$r$ [cm]')
plt.ylabel(r'$\Delta T \equiv T(t_i) - T(0)$ [K]')
plt.show()

