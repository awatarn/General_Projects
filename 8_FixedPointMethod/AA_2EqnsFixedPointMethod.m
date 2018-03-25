%% 
% SOLVE A SYSTEM OF NONLINEAR EQUATIONS BY USING FIXED-POINT METHOD
% CREATED ON 24 OCT 2017
% AUTHOR: APIWAT WISITSORASAK
%
%% Clear
clear
close all
clc
%% Set parameters
NStep = 10;

% x10 = 0.0;
% x20 = 0.1;

x10 = 2.0;
x20 = 0.0;

%% Main loop
i = 0;
temp1 = sprintf('i = %2d x1 = %12.7f x2 = %12.7f',i,x10,x20);
disp(temp1)
for i=1:1:NStep
    x1p = func1(x10,x20);
    x2p = func2(x10,x20);
    temp1 = sprintf('i = %2d x1 = %12.7f x2 = %12.7f',i,x1p,x2p);
    disp(temp1)
    x10 = x1p;
    x20 = x2p;
end

%%

