#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Fixed-point iteration method
Solve g(x) = x
@author: Apiwat Wisitsorasak
12 Feb 2018
"""
import numpy as np

def gn(x):
    return np.sqrt(10 - x**3)*0.5
# Inputs
p0  = 2         # Initial guess
TOL = 1e-4      # Tolerance
N0  = 100       # Maximum number of iteration
# Main computation
i = 1
while i<=N0:
    temp1 = 'i = ' + repr(i) + ' p0 = ' + repr(p0)
    print(temp1)
    p = gn(p0)
    if (np.abs(p-p0)<TOL): # the solution is converged
        temp1 = 'Solution: p = ' + repr(p) + ' i = ' + repr(i)
        print(temp1)
        break
    p0 = p
    i = i + 1
    
    
# Check if i>=N0
if i==N0:
    temp1 = 'The solution is not converged. Iteration exceeds the limit N0.'
    print(temp1)
