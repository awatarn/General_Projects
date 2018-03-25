#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Mar  7 09:41:42 2018

@author: vitreloy
"""
import scipy.optimize as opt
from numpy import exp
import timeit

st1 = timeit.default_timer()

def f(variables) :
    (x,y) = variables

    first_eq = x + y**2 -4
    second_eq = exp(x) + x*y - 3
    return [first_eq, second_eq]

solution = opt.fsolve(f, (0.1,1) )
print(solution)


st2 = timeit.default_timer()
print("RUN TIME : {0}".format(st2-st1))