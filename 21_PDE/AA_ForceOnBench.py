#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Feb 24 22:26:08 2018
Solve the curvature of a bench
YI u''(x) = f(x)

@author: Apiwat Wisitsorasak
"""
import numpy as np
import matplotlib.pylab as plt

# Parameters
f0 = 200    # Force constant 1
x0 = 0.25   # Force constant 2
L  = 3.0    # Beam length
w  = 0.20   # Beam width
t  = 0.03   # Beam tickness
rho= 3.0    # Density
Y  = 1e9    # Elastic modulus
g  = 9.8    # g
# Parameters for numerics
N  = 99     # Number of grid
dx  = L/N   # Width of spatial grid
#
I  = (t**3)*w/3.0   # Moment of inertia
fx = np.zeros((N,1))    # Force distrituion function
x  = range(N)           # Spatial index
# Compute force distribution function
i = 0
while i<N:
    xi = (i+1)*dx
    if np.abs(xi - L/2)<=x0:
        fx[i] = -f0*(np.exp(-(xi-L/2.0)**2/x0**2) - np.exp(-1)) - rho*g
    else:
        fx[i] = -rho*g
    i = i+1
# Main calculation
A = np.zeros((N,N))     # Matrix coefficients A
B = np.zeros((N,1))     # Vector B
# While loop
k = 0
while k<=(N-1):
    left = 1
    middle  = -2
    right = 1
    if k==0: # (Left boundary)
        A[k][k] = middle
        A[k][k+1] = right
    elif k==(N-1): # (Right boundary)
        A[k][k-1] = left
        A[k][k] = middle
    else:  # (Any other points)
        A[k][k-1] = left
        A[k][k] = middle
        A[k][k+1] = right
    B[k] = dx**2*fx[k]/Y/I
    k = k+1
# End of while loop
u = np.dot(np.linalg.inv(A),B)  # Solve for u(x)
plt.plot(x,u,'o')               # Plot graph of u(x)
plt.xlabel('x (index)')
plt.ylabel('u(x)')
