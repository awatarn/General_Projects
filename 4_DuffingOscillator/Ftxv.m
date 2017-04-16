function [fx,fv] = Ftxv(alpha,beta,delta,gamma,omega,t,x,v)
%% SOLVE DUFFING EQUATION WITH RK4 METHOD
%  A. WISITSORASAK CREATED ON 16 APR 2017
%  \ddot x + \delta \dot x + \alpha x + \beta x^3 = \gamma \cos(\omega t)
%  This function is the right hand side of the dynamical equation:
%  \dot y = f(t,y)
%  Here y is a vector column with 2 elements
%  \dot x = v
%  \dot v = -\delta v - \alpha x - \beta x^3 + \gamma \cos (\omega t)
%
%  References: 
%  https://en.wikipedia.org/wiki/Duffing_equation
%  https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods
%%
    fx = v;
    fv = -delta*v - alpha*x - beta*x*x*x + gamma*cos(omega*t);
end