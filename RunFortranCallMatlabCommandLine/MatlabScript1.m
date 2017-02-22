clear
close all
clc
%%
c = clock;
hour = c(4);
min = c(5);
seconds = c(6);
temp1 = sprintf('%2d : %2d : %2.5f',hour,min,seconds);
disp(temp1)

exit