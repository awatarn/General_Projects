clear 
close all
clc
%%
a=ones(8,8).*0;

a(1,1) = 7;
a(1,3) = 1;
a(1,6) = 2;
a(1,7) = 7;
a(2,2) = -4;
a(2,3) = 8;
a(2,5) = 2;
a(3,3) = 1;
a(3,8) = 5;
a(4,4) = 7;
a(4,7) = 9;
a(5,5) = 5;
a(5,6) = 1;
a(5,7) = 5;
a(6,6) = -1;
a(6,8) = 5;
a(7,7) = 11;
a(8,8) = 5;
b = a';
for i=1:8
    b(i,i) = 0;
end
A = a+b
B = ones(8,1);
X = inv(A)*B