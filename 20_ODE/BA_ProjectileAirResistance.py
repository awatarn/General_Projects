#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Feb 21 15:44:41 2018
Solve projectile motion with air resistance

@author: Apiwat Wisitsorasak
"""
import numpy as np
import matplotlib.pyplot as plt
# Inputs

A = 0.93
v0 = 67
rho = 1.2
m = 500
theta0 = 42.5
c = 1
g = 9.8

def fx(vx,vy):
    return vx

def fy(vx,vy):
    return vy

def fvx(vx,vy):
    global A, V0, rho, m, c
    return  -c*A*rho*np.sqrt(vx*vx + vy*vy)*vx/m

def fvy(vx,vy):
    global g
    return -g -c*A*rho*np.sqrt(vx*vx + vy*vy)*vy/m
    



# 
dt = 0.00001
NTimeStep = 1000000

#
x = 0
y = 0
vx = v0*np.cos(theta0*np.pi/180)
vy = v0*np.sin(theta0*np.pi/180)
ti = 0

tt = [ti]
xx = [x]
yy = [y]
vvx = [vx]
vvy = [vy]


i = 1
while i<=NTimeStep:
    kx1 = dt*fx(vx,vy)
    ky1 = dt*fy(vx,vy)
    kvx1 = dt*fvx(vx,vy)
    kvy1 = dt*fvy(vx,vy)
    
    kx2 = dt*fx(vx+0.5*kvx1,vy+0.5*kvy1)        
    ky2 = dt*fy(vx+0.5*kvx1,vy+0.5*kvy1)
    kvx2 = dt*fvx(vx+0.5*kvx1,vy+0.5*kvy1)
    kvy2 = dt*fvy(vx+0.5*kvx1,vy+0.5*kvy1)
        
    x = x + kx2
    y = y + ky2    
    vx = vx + kvx2    
    vy = vy + kvy2
    
    ti = ti + dt
    
    tt.append(ti)
    xx.append(x)
    yy.append(y)
    vvx.append(vx)
    vvy.append(vy)
    
    if y<0:
        print('The object hits the ground at ti = '+repr(ti)+' i = '+repr(i))
        print('Range of the object is x_max = '+repr(x))
        print('Highest height is y_max = '+repr(max(yy)))
        break
    i = i + 1

plt.plot(xx,yy,'r--')
plt.xlabel('x')
plt.ylabel('y')
plt.show()