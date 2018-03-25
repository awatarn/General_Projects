#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Feb 14 21:42:22 2018
Gaussian elimination
Solve A*X = B
A = N*N matrix
X = solution
B = N*1 or 1*N matrix

@author: Apiwat Wisitsorasak
"""
import numpy as np

# Input

#A = np.array([[2.0,1.0,-1.0],[-3.0,-1.0,2.0],[-2.0,1.0,2.0]]) # Coefficient matrix
#B = np.array([8.0,-11.0,-3.0])                    # Vector B

A = np.array([[1,2,4,6],[2,3,0,1],[4,1,1,0],[5,0,2,1]],dtype='f') # Coefficient matrix
B = np.array([[1],[1],[1],[1]],dtype='f')      # Vector B

print('Coefficient matrix (A):')
print(A)
print('R.H.S vector (B):')
print(B)


# Internal variables
N = A.shape[0]          # Dimension of A
x = np.ones([N,1])*0    # Solution

# Perform Gaussian elimination procedure
# Forward elimination
k=0
while k<N-1:
    i=k+1
    while i<=N-1:
        j = k+1
        ratio = A[i][k]/A[k][k]
        while j<=N-1:
            A[i][j] = A[i][j] - A[k][j]*ratio
            j = j+1
        B[i] = B[i] - B[k]*ratio
        i = i+1
    k = k+1
        
# Backward substitution
k=N-1
while k>=0:
    x[k] = B[k]/A[k][k]
    i = k+1
    while i<N:
        x[k] = x[k] - (A[k][i]/A[k][k])*x[i]
        i = i+1
    k = k-1

# Print solution
print('Solution (x):')
print(x)


#            print('({0}{1}) = ({2}{3}) - ({4}{5})*({6}{7})'.format(i,j,i,j,k,j,i,k))