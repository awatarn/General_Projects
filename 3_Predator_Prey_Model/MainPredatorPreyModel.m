%% PREDATOR-PREY MODEL
%  A. WISITSORASAK CREATED ON 16 APR 2017
%
%  dx/dt = alpha*x - beta*x*y
%  dy/dt = delta*x*y - gamma*y
%  x CAN BE CONSIDERED AS PREY SPECIES. y CAN BE CONSIDERED AS PREDATOR
%  SPECIES. 
%  NUMERICAL METHOD: FINITE DIFFERENECE WITH EXPLICIT TIME DEPENCE
%  REFERENCE: WIKIPEDIA
%
%% CLEAR
clear
close all
clc

%% INPUT PARAMETERS
% oscillation
alpha = 2/3;
beta  = 4/3;
gamma = 1;
delta = 1;

x0    = 0.9;
y0    = 0.9;

dt    = 0.001;
tmin  = 0;
tmax  = 30;

dtplot= 0.1;

%% INItjALIZAtjON
tj = tmin;
tjplot = tmin;
xj = x0;
yj = y0;

xdata = [];
ydata = [];
tdata = [];
%% MAIN LOOP
while tj<=(tmax+dt)
    if tj>=tjplot
        xdata = [xdata; xj];
        ydata = [ydata; yj];
        tdata = [tdata; tj];
        tjplot = tjplot+dtplot;
    end
    
    xj1 = xj + dt*(alpha*xj - beta*xj*yj);
    yj1 = yj + dt*(delta*xj*yj - gamma*yj);
    
    xj = xj1;
    yj = yj1;
    tj = tj+dt;
end

%% VISUALIZAtjON
figure();
plot(tdata,xdata,'r-','LineWidth',2);
hold on
plot(tdata,ydata,'b-','LineWidth',2);
hold off
legend('x (prey)','y (predator)');
xlabel('Time');
ylabel('x or y');

figure();
plot(xdata,ydata,'k-','LineWidth',2);
xlabel('x (prey)');
ylabel('y (predator)');
