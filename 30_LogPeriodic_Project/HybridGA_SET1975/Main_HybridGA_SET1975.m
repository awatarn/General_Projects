clc
clear
close all
%% Set tc, omega, phi, z
global t0 tmax

t0    = 725000; % Initial time for stripping the time data
tmax  = 729000; 
% Good1 (722000, 729000), (724000, 729000)
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
LB    = [tmax+1 -2*pi 0.001 0.01];  % Lower bound
UB    = [tmax+2000 2*pi 100 1.00];  % Upper bound
ConstraintFunction = @simple_constraint;

% options = optimoptions(@ga,'MutationFcn',@mutationadaptfeasible);
% options = optimoptions(options,'PlotFcn',{@gaplotbestf,@gaplotmaxconstr}, ...
%     'Display','iter');
% [x,fval] = ga(ObjectiveFunction,nvars,[],[],[],[],LB,UB, ...    
%               ConstraintFunction,options);

% fminconOptions = optimoptions('fmincon','Display','iter','Algorithm','sqp');
fminconOptions = optimoptions('fmincon','Display','iter-detailed','Algorithm','active-set');
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

temp1 = sprintf('tc - t_final = %f days',x(1) - tmax);
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
tCalc = ti(indextc);
yCalc = PredictedPrice(bhat,x,ti(indextc));
plot(ti,yi,'r.',...
    tCalc,yCalc,'b-',...
    xtemp,ytemp,'--r','linewidth',2);

%% Calculate R-Square

Rsq = RSquare(yi, yCalc);

temp1 = sprintf('RSquare      = %f',Rsq);
disp(temp1)

%% Convert Datenum to DecimalYear

% tj = ti;
% deciyear = DatenumToDecimalYear(ti);

%% Plot - comparison between prediction and experimental values
% 1.0 Experimental data
tmax2 = tc + 500;

tempz = find(ExcelData(:,7)>=tmax2);
Last2  = tempz(1);

CloseP2 = ExcelData(Last:Last2,5);
DateList2 = ExcelData(Last:Last2,7);

yi2 = log10(CloseP2); 
ti2 = DateList2;       % yi2 & ti2 are data that are not included for fitting,
                       % but it will appear on the graph for comparison.
                       
% 2.0 Prediction data (extrapolation)                       
CloseP3 = ExcelData(First:Last2,5);
DateList3 = ExcelData(First:Last2,7);

yi3 = log10(CloseP3);
ti3 = DateList3;


figure();
indextc3 = find(ti3<(x(1)+0));
tc = x(1);
xtemp = [tc tc];
ytemp = [min(yi) max(yi)];

tCalc3 = ti3(indextc3);
yCalc3 = PredictedPrice(bhat,x,ti3(indextc3));

plot(DatenumToDecimalYear(ti),yi,'r.',...
    DatenumToDecimalYear(ti2),yi2,'k.',...
    DatenumToDecimalYear(tCalc3),yCalc3,'b-',...
    DatenumToDecimalYear(xtemp),ytemp,'--b','linewidth',2);
xlabel('Year');
ylabel('Log(Index)');
txt = {'\leftarrow t_c'};
text(DatenumToDecimalYear(xtemp(2)),ytemp(1)*1.05,txt,'FontSize',18)


%https://www.mathworks.com/help/gads/examples/constrained-minimization-using-the-genetic-algorithm.html