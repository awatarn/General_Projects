#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Feb 21 12:29:33 2018
Integration by Simpson rule

@author: Apiwat Wisitsorasak
"""

import numpy as np

def fn(x):
    return np.exp(x)

def fn_analytic(a,b):
    return np.exp(b) - np.exp(a)

# Inputs
N = 100        # Number of slices
a = 0.0         # Left boundary
b = 1.0         # Right boundary
h = (b - a)/N   # Width of each slice

# Do integration
s = 0
i = 0
while i<=(N/2-1):
    x2i = a+2*i*h
    x2i1 = a+(2*i+1)*h
    x2i2 = a+(2*i+2)*h
    s = s + (h/3.0)*(fn(x2i) + 4*fn(x2i1) + fn(x2i2))
    i = i+1

# Print output
print('The solution of the integration is ' + repr(s))
print('The analytical solution is ' + repr(fn_analytic(a,b)))
Deviation = (s - fn_analytic(a,b))/fn_analytic(a,b)*100
print('Percentage deivation = ' + repr(Deviation)+ '%')