# -*- coding: utf-8 -*-
"""
Created on Mon Mar  9 20:30:45 2020

@author: Msi
"""

import numpy as np
import matplotlib.pyplot as plt

#parameter
R        = 0.65      # Major radius [m]
a        = 0.20      # Minor radius [m]
Bt       = 1.5       # Toroidal magnetic field [T]
Ai       = 2.5       # Average atomic mass number of plasma ions(D-T)
I        = 2         # Plasma current [MA]
zeff     = 1.5       # Effective plasma charge
os       = 1         # Transparency factor
sigma    = 1         # Plasma triangularity
K        = 1         # Plasma elongation
T        = 2         # Temperature [keV]
F        = 1         # Scale factor
alphaN   = 0.5
alphaT   = 1.0
YNC      = 2

V        = 2*(np.pi**2)*(a**2)*R*K # Plasma volume [m**3]
n20      = 0.75                  # Normalized arerage electron density [*10^20 m^-3]
ne       = n20*(10^20)           # Electron density [m^-3]
T10      = T/10                  # Temperature [*10 keV]
T10C     = (1+alphaN)*(1+2*alphaN+3*alphaT)/((1+alphaN+alphaT)*(1+2*alphaN+2*alphaT))
Falpha   = (T10/T10C)**3
fDT      = 0.9
CS       = 6.2e-2
CB       = (1.6e-2)*zeff*((1+alphaN)**(3/2))*((1+alphaN+alphaT)**(1/2))/(1+2*alphaN+0.5*alphaT)
COH      = (9.33e-5)*zeff*(((1+alphaN)/(1+alphaN+alphaT))**(3/2))*(1+3*alphaT/2)
Calpha   = 0.16*(fDT**2)*((1+alphaN)**2)*((1+2*alphaN+3*alphaT)**2)/((1+2*alphaN+2*alphaT)**3)

""" ========================== """
""" FIND NET HEATING POWER (P) """
""" ========================== """
Nmax     = 1000                                         # Number of time step
Paux     = np.ones(Nmax)
j = 0
while j < Nmax:
    if j > 400:
        Paux[j] = 5
    if j > 800:
        Paux[j] = 10
print(Paux)                                            # Auxiliary power [MW]
POH      = V*COH*YNC*((I/(K*(a**2)))**2)/(T10**(3/2))*np.ones(Nmax)
Palpha   = V*Calpha*(n20**2)*Falpha*np.ones(Nmax)
PBrad    = V*CB*(n20**2)*(T10**(1/2))*np.ones(Nmax)
Psync    = V*CS*n20*T10*(Bt**2)*os*np.ones(Nmax)

P        = Paux+POH+Palpha-PBrad                         # Net heating power [MW]

""" ========================== """
""" FIND CONFINEMENT TIME (TE) """
""" ========================== """
TE       = 0.0481*(I**0.85)*(R**1.2)*(a**0.3)*(K**0.5)*(n20**0.1)*(Bt**0.2)*(Ai**0.5)*(np.abs(P)**-0.5)*F

""" ====================================== """
""" FIND TOTAL PLASMA THERMAL ENERGY (Wth) """
""" ====================================== """
Wth      = 0.24*n20*T10*(1+1)*V # [MJ]

""" ============ """
""" Eulor Method """
""" ============ """
Pcon     = V*0.24*(n20**2)*T10*(1+1)/(n20*TE)*np.ones(Nmax)
W0       = 100       # Internal energy
dt       = 0.001     # Time step
Nmax     = 1000    # Number of time step
Warray = np.ones(Nmax)
tarray = np.ones(Nmax)
#Equation
def fn(w,t,A,B,C,D,E,F):
    return -(w/t)-A-B-C+D+E+F
#loop
W = W0
i = 0
while i < Nmax:
    Warray[i] = W
    tarray[i] = i*dt
    W_dt =W+(dt*fn(Wth,TE,Psync[i],Pcon[i],PBrad[i],POH[i],Palpha[i],Paux[i])) 
    W = W_dt
    i=i+1
    
plt.plot(tarray,Warray)
plt.xlabel('Time')
plt.ylabel('Thermal Energy')
plt.grid()
plt.show()













