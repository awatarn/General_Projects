clc
clear
close all
%% Set tc, omega, phi, z
global t0 tmax

t0    = 722000; %good 38850; %38120; % 37691; % Initial time for stripping the time data
tmax  = 729000; % 39500;
% tc    = 38100;
% omega = 5.00;
% phi   = pi;
% z     = 0.33;

%% Load raw data

global ExcelData
% filename = 'SET50-1-Day.xlsx';
filename = '../SET_EOD_1975_2018.xlsx';
ExcelData = xlsread(filename);


%% Optimization F_theta
% x = [38100; pi; 5.00; 0.42];

ObjectiveFunction = @Ftheta;
nvars = 4;                  % Number of variables
LB    = [tmax+1 -2*pi 0.01 0.01];  % Lower bound
UB    = [tmax+1000 2*pi 200 1];  % Upper bound
ConstraintFunction = @simple_constraint;

% options = optimoptions(@ga,'MutationFcn',@mutationadaptfeasible);
% options = optimoptions(options,'PlotFcn',{@gaplotbestf,@gaplotmaxconstr}, ...
%     'Display','iter');
% [x,fval] = ga(ObjectiveFunction,nvars,[],[],[],[],LB,UB, ...    
%               ConstraintFunction,options);

fminconOptions = optimoptions('fmincon','Display','iter','Algorithm','sqp');
options = optimoptions('ga','HybridFcn',{@fmincon, fminconOptions}); 
[x,fval] = ga(ObjectiveFunction,nvars,[],[],[],[],LB,UB, ...    
              ConstraintFunction,options);
          
%% Display the outputs

temp1 = sprintf('fval  = %f',fval);
disp(temp1)
temp1 = sprintf('tc    = %f',x(1));
disp(temp1)
temp1 = sprintf('Phi   = %f',x(2));
disp(temp1)
temp1 = sprintf('omega = %f',x(3));
disp(temp1)
temp1 = sprintf('z     = %f',x(4));
disp(temp1)

tempy = find(ExcelData(:,7)>=t0);
First = tempy(1);

tfinal = tmax;
tempz = find(ExcelData(:,7)>=tfinal);
Last  = tempz(1);
OpenPrice = ExcelData(First:Last,2);

CloseP = ExcelData(First:Last,5);
DateList = ExcelData(First:Last,7);

yi = log10(CloseP);
ti = DateList;

%%
global bhat
figure();
indextc = find(ti<(x(1)+1000));
tc = x(1);
xtemp = [tc tc];
ytemp = [min(yi) max(yi)];
plot(ti,yi,'r.',...
    ti(indextc),PredictedPrice(bhat,x,ti(indextc)),'b-',...
    xtemp,ytemp,'--r','linewidth',2);


%%

%https://www.mathworks.com/help/gads/examples/constrained-minimization-using-the-genetic-algorithm.html