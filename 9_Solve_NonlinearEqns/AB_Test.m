%% Clear
close all
clear
% clc
%% FUNCTION IMPLEMENTATION
fun = @gxtest;
x0 = [0,0];
x = fsolve(fun,x0)

%% 
function F = gxtest(x)
    hx = exp(-exp(-(x(1) + x(2))));
    fx = x(2)*(1+x(1)^2);
    
    F(1) = hx - fx;
    F(2) = x(1)*cos(x(2)) + x(2)*sin(x(1)) - 1.0;
end
