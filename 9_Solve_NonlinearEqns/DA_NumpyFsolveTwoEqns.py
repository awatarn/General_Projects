#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Mar  7 09:41:42 2018

@author: vitreloy
"""

from scipy.optimize import fsolve
import math

def equations(p):
    x, y = p
    return (x+y**2-4, math.exp(x) + x*y - 3)

x, y =  fsolve(equations, (1, 1))

print(repr(x)+' '+repr(y))
print(equations((x,y)))
#print equations((x, y))