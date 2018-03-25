#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Feb 21 11:23:50 2018
Integration by the trapezoidal rule

@author: Apiwat Wisitsorasak
"""
import numpy as np

def fn(x):
    return np.exp(x)

def fn_analytic(a,b):
    return np.exp(b) - np.exp(a)

# Inputs
N = 1000        # Number of slices
a = 1.0         # Left boundary
b = 5.0         # Right boundary
h = (b - a)/N   # Width of each slice

# Do integration
s = 0
i = 0
while i<=N-1:
    xi = a+i*h
    xi1 = a+(i+1)*h
    s = s + (h/2.0)*(fn(xi) + fn(xi1))
    i = i+1

# Print output
print('The solution of the integration is ' + repr(s))
print('The analytical solution is ' + repr(fn_analytic(a,b)))
Deviation = (s - fn_analytic(a,b))/fn_analytic(a,b)*100
print('Percentage deivation = ' + repr(Deviation)+ '%')