#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Mar  7 09:41:42 2018

@author: vitreloy
"""
import scipy.optimize as opt
from numpy import exp
import numpy as np
# PARAMETERS
gamma = 7.5
a = 0.20
R = 0.65
q = 3.5
L = np.pi*q*R
psi   = a/(q*R)
mi    = 1.67e-27
sigma = 1e-19 # Effective cross-section
A_kappa = 2000
S       = 4*np.pi*np.pi*a*R
kB      = 1.38e-23
eVtoK   = 11604.519
ee      = 1.6e-19
d_SOL   = 0.08         # [m]
D_perp  = 1.0          # [m**2/s]
chi_perp = D_perp*3.0
P_heat   = 1e6         # [W]
q_perp   = P_heat/S
n_S      = 1e-1        # [10**20 cm**-3]

# INITIAL GUESS
T_S0 = 20.0
T_L0 = 10.0
n_L0 = 10e-1
Gamma_LCMS0 = 1.0e1    # [x10^20 1/(m^2 s)]
delta0 = 0.25;

def F(variables) :
    (Ts,Tl,nl,gamma_perp,delta) = variables
    V_S = np.sqrt(kB*2.0*Tl*eVtoK/mi)
    n1 = 5.0
    n2 = 13.7
    E_i = (35.0 - 21.4*np.exp(-n1/nl))*np.exp((5.45/Tl)*np.exp(-(nl/n2)**0.26))*eVtoK*kB
    T_C = q_perp/(5.0*gamma_perp*1e20*kB*eVtoK)
    
    Eq1 = P_heat/1e6 - 4*np.pi*R*delta*(gamma*Tl*eVtoK*kB + E_i)*nl*1e20*V_S*np.sin(psi)/1e6
    Eq2 = gamma_perp/delta - 4*np.pi*R*delta*V_S*np.sin(psi)*np.exp(-nl*1e20*sigma*d_SOL)
    Eq3 = 2.0*nl*Tl - n_S*Ts
    Eq4 = 0.5*(T_C**(2.5))*(np.log((1+np.sqrt(Ts/T_C))/(1-np.sqrt(Ts/T_C))) - np.log((1+np.sqrt(Tl/T_C))/(1.0-np.sqrt(Tl/T_C))))       \
        - (Ts**2.5 - Tl**2.5)/5.0 - T_C*(Ts**1.5-Tl**1.5)/3.0 \
        - T_C**2*(np.sqrt(Ts)-np.sqrt(Tl)) \
        - (5.0*L**2/A_kappa/delta/4.0)*gamma_perp*kB
    Eq5 = delta*delta - (2.0*L/V_S)*(chi_perp + 3.0*D_perp)/(gamma + (E_i/(kB*Tl*eVtoK)))
    return [Eq1, Eq2, Eq3, Eq4, Eq5]
    
    
guess0 = (T_S0,T_L0,n_L0,Gamma_LCMS0,delta0)
solution = opt.fsolve(F,guess0)
print(solution)




