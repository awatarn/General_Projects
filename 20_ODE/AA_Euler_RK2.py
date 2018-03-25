#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Feb 21 12:54:43 2018
Solve ODE dN/dt = - \lambda N by Euler method
@author: Apiwat Wisitsorasak
"""
import matplotlib.pyplot as plt
import numpy as np

def fn(y):
    DecayConstant = 10.0
    return -DecayConstant*y 

# Inputs
ta = 0.0        # Initial time
tb = 0.5        # Final time
N  = 10         # Number of time steps
w0 = 1e23       # Initial number of nuclei

# Main calculation
h = (tb-ta)/N   # Time step

i = 1           # Counter for while loop
ti = ta         # Set t_i = t_a
w = w0          # Set w(t_i) = w0
r = w0

tt = [ta]       # List of time
ww = [w0]       # List of computed nuclei (Euler's method)
yy = [w0]       # List of analytical solution
rk2 = [w0]      # List of computed nuclei (RK2 method)

while i<=N:
    w = w + h*fn(w)
    
    k1 = h*fn(r)
    k2 = h*fn(r+0.5*k1)
    r  = r+k2
    
    ti = ti + h     # Compute time t_i
    tt.append(ti)   # Append data to a list (for plotting)
    ww.append(w)    # Append data to a list (for plotting)
    yy.append(w0*np.exp(-10*(ti-ta)))   # Append data to a list (for plotting)
    rk2.append(r)
    i = i+1

# Visualization
plt.plot(tt,yy,'r-',tt,ww,'bo--',tt,rk2,'k^--')
plt.xlabel('Time')
plt.ylabel('N(t)')
plt.legend(('Analytical','Euler','RK2'),loc='upper right',shadow=False)
plt.grid(True)
plt.show()
