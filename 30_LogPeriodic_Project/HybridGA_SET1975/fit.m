close all
clear
clc

%%


filename = 'SET50-1-Day.xlsx';
ExcelData = xlsread(filename);

First = 2029;
Last = 4000;
OpenPrice = ExcelData(First:Last,2);


    
OpenP = ExcelData(First:Last,2);
CloseP = ExcelData(First:Last,5);
DateList = ExcelData(First:Last,1);

yexp = CloseP ;
t = DateList;

fun = @(x) x(1) - x(2)*(x(4)-t).^x(7) + ...
    x(3)*((x(4) - t).^x(7)).*cos(x(5)*log10(x(4) - t) + x(6)) - yexp;


d0 = [700, 500, 48, 30000, 5.56, 5.5, 0.3];
% options.Algorithm = 'levenberg-marquardt';
% Param = lsqnonlin(@(x) [real(fun(x)), imag(fun(x))],d0,[],[],options)
options = optimoptions('lsqnonlin','Display','iter')
Param = lsqnonlin(fun,d0,[],[],options)

Param(4)


plot(t,yexp,'ro',t,fun(Param)+yexp,'bo');
