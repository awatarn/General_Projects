%% SOLVE TRIDIAGONAL MATRIX
close all
clear
clc
%%
A = ones(5,5).*0;
A(1,1) = 1;
A(1,2) = 4;
A(2,1) = 3;
A(2,2) = 4;
A(2,3) = 1;
A(3,2) = 2;
A(3,3) = 3;
A(3,4) = 4;
A(4,3) = 1;
A(4,4) = 3;
A(4,5) = 1;
A(5,4) = 2;
A(5,5) = 3;

A(1,5) = 1;
A(5,1) = 2;


A
B = ones(5,1);
X = inv(A)*B

% clc
% a = [4 1 0 0; 2 3 4 0; 0 1 3 1; 0 0 2 3]
% b1 = ones(4,1);
% x1 = inv(a)*b1
% 
% b2 = ones(4,1).*0;
% b2(1) = -3;
% b2(4) = -2;
% x2 = inv(a)*b2

