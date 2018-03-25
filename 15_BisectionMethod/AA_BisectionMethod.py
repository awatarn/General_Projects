# -*- coding: utf-8 -*-
"""
Solve f(x) = 0 by bisection method

Apiwat Wisitsorasak
12 Feb 2018
"""
import matplotlib.pyplot as plt
import numpy as np

# Define a function f(x)
def fn(x):
    return x**3 + 4*x**2 - 10

# Input
a = 1  # Left boundary;  f(a) < 0
b = 2  # Right boundary; f(b) > 0
TOL = 1e-8   # tolerance
N0 = 100     # maximum number of iteration

# Visualization
t = np.arange(0.0, 2.0, 0.01)
w = fn(t)
# Note that using plt.subplots below is equivalent to using
# fig = plt.figure and then ax = fig.add_subplot(111)
fig, ax = plt.subplots()
ax.plot(t, w)

ax.set(xlabel='x', ylabel='f(x)',
       title='x**3 + 4*x**2 - 10')
ax.grid()

#fig.savefig("test.png")
plt.show()


# Main calculation
FA = fn(a)
i  = 1
while i <= N0:
    p = (a+b)/2     # Compute the midpoint
    FP = fn(p)      # Compute the function at point p
    if (FP == 0) or  ((b-a)<TOL):     # The solution is found
        temp1 = 'The solution is converged at i = ' + repr(i)
        print(temp1)
        temp1 = 'The solution x of f(x) = 0 is x = ' + repr(p)
        print(temp1)
        break
    if (FA*FP)>0:   # Check the sign of FA*FP
        a = p
        FA = FP
    else:
        b = p
    i = i + 1       # Increase the dummy index of while loop
    
# Check if i>=N0
if i==N0:
    temp1 = 'The solution is not converged. Iteration exceeds the limit N0.'
    print(temp1)    
    