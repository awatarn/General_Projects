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
# Parameters (All variables are in SI units)
c = 789.0           # Heat capacity of concrete
sigma = 1.00        # Emissivity ? [W/mK]
rho = 2.00e3        # Density [kg/m**3]
kappa = sigma/c/rho # [m**2/s]
a = 0.25            # Cut-off radius [m]
xmin = 0.0          # minimum x [m]
xmax = 1.0          # maximum x [m]
T0 = 1.0            # Temperature constant [K]
tau0 = 1.0*100*3600*24*365 # Time constant [s]
TE = 300            # Ambient temperature [K]
# 
NGrid = 100                 # Number of grids
dx = (xmax - xmin)/NGrid    # Grid size
NTime = 1000                # Number of time steps
dt = 3600*24*30             # Time step
# 
Ta = np.ones((NGrid,1))*TE  # Initial T (or T at step n)
Tb = np.zeros((NGrid,1))    # T at step n+1
AA = np.zeros((NGrid,NGrid))# Coeffient matrix
BB = np.zeros((NGrid,1))    # Right-hand vector
Tdiff = Ta - np.ones([NGrid,1])*TE # Temperature difference (compare with initial T)

TD = Tdiff.T        # Transpose 
# Main computation
ti = dt
tn = 1
while tn<=NTime:            # Main time loop
    xi = 1  
    while xi<=NGrid:        # Spatial variable loop
        i = xi-1            # Index of matrix start with 0, not 1
        gi = kappa*dt/((xi*dx)*dx*dx)   # gamma
        if (xi*dx)<=a:
            S = (T0/a/a)*np.exp(-ti/tau0)   # Source term (inside nuclear waste rod)
        else:
            S = 0.0                         # Source term (outside nuclear waste rod)
        if xi == 1:                         # Left boundary
            A = 0.0
            B = 1.0 + gi*((xi+0.5)*dx) + gi*((xi-0.5)*dx) - gi*((xi-0.5)*dx)
            C = -gi*((xi+0.5)*dx)
            D = Ta[i] + kappa*dt*S
            AA[i][i]   = B
            AA[i][i+1] = C
            BB[i]      = D            
        elif xi == NGrid:                   # Right boundary
            A = -gi*((xi-0.5)*dx)
            B = 1.0 + gi*((xi+0.5)*dx) + gi*((xi-0.5)*dx)
            C = 0.0
            D = Ta[i] + kappa*dt*S + gi*((xi+0.5)*dx)*TE
            AA[i][i-1] = A
            AA[i][i]   = B
            BB[i]      = D            
        else:                               # Any other i
            A = -gi*((xi-0.5)*dx)
            B = 1 + gi*((xi+0.5)*dx) + gi*((xi-0.5)*dx)
            C = -gi*((xi+0.5)*dx)
            D = Ta[i] + kappa*dt*S
            AA[i][i-1] = A
            AA[i][i]   = B
            AA[i][i+1] = C
            BB[i]      = D
        xi = xi + 1
    Tb = np.dot(np.linalg.inv(AA),BB)       # Solve for T_n+1
    Ta = Tb                                 # Update variables
    Tdiff = Ta - np.ones([NGrid,1])*TE      # Compute T_diff
    TD = np.append(TD,Tdiff.T,axis=0)       # Store data
    ti = ti + dt
    tn = tn + 1
# End of while loop
plt.figure(1)
plt.imshow(TD)

i = 1
R = np.ones(NGrid)
while i<=NGrid:
    R[i-1] = i*dx
    i = i + 1
    
plt.figure(2)    
plt.plot(R,TD[0],'r--',R,TD[10],'go',R,TD[50],'bs',R,TD[500],'k*',R,TD[1000],'m.')
plt.legend(('0','10 month','50 months','500 months','1000 months'))
plt.xlabel(r'$r$ [cm]')
plt.ylabel(r'$\Delta T \equiv T(t_i) - T(0)$ [K]')
plt.show()

