%% This code solves a system of two nonlinear equations.
%  Further infomation: https://www.mathworks.com/help/optim/ug/fsolve.html
%  Author: Apiwat Wisitsroasak
%  Date 2 Nov 2017
%%
clear
close all
clc
%%
fun = @root2d;
x0 = [0,0];
options = optimoptions('fsolve','Display','iter');
x = fsolve(fun,x0,options)