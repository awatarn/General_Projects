%%
% This program solves the two point model in SOL tokamak for n_t, T_t, n_u.
% The equations are taken from Eq.5.4 - 5.7 from Stangby's book.
% It takes q_parallel and n_u as inputs & returns n_t, T_t, T_u as outputs.
% n_u & n_t are in units of 1E20 m**-3
% T_u & T_t are in units of eV
% q_par is in unit of W/m**2
% L (~2*pi*q) [m]
% Late update: 3 November 2017
% Author: Apiwat Wisitsorasak
%
%
%% CLEAR
clear
close all
clc
%% PARAMETERS
global nu qpar k mi gamma L k0e e

k     = 1.380648e-23;   % Bolzmann constant [J/K]
mi    = 1.672e-27;      % ion mass [kg]
gamma = 7;              % heat coefficient ration [-]
L     =  100;           % average length of particles ~ 2*pi*q [m]
k0e   = 2000;           % ?
e     = 1.6e-19;        % fundamental charge [C]

%% INPUTS
nu = 0.4; % Density at the stagnation point [x1E20 m**-3]
qpar = 100e6; % Paralle heat conduction coefficient [W/m**2]

%% MAIN PART (COMPUTATION ROUTINE)
x0 = [0.3,96,10];       % Initial guess [nt,Tt,Tu]

fun = @TwoPoint;
options = optimoptions('fsolve','Display','iter-detailed');
x = fsolve(fun,x0,options);

%% PRINT-OUT 
temp1 = sprintf('Density at target               (nt): %.4fE20 m**-3',x(1));
disp(temp1)
temp2 = sprintf('Temperature at target           (Tt): %.4f    eV',x(2));
disp(temp2)
temp3 = sprintf('Temperature at stagnation point (Tu): %.4f  eV',x(3));
disp(temp3)

%% FUNCTION IMPLEMENTATION
function F = TwoPoint(x)
    % x = [nt [1e20 m**3], Tt [eV], Tu [eV]]
    
    global nu qpar k mi gamma L k0e
    
    F(1) = 2*x(1)*x(2) - nu*x(3);
    F(2) = x(3)^(7/2) - x(2)^(7/2) - (7/2)*qpar*L/k0e;
    F(3) = qpar - gamma*(x(1)*1e20)*k*(x(2)*11604.519)*sqrt(2*k*(x(2)*11604.519)/mi);    
end