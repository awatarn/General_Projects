%% SOLVE DUFFING EQUATION WITH RK4 METHOD
%  A. WISITSORASAK CREATED ON 28 APR 2017
%  \ddot \theta = - \frac{g}{l} \sin(\theta)
%
%  References: 
%  https://en.wikipedia.org/wiki/Duffing_equation
%  https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods
%
%% CLEAR
close all
clear
clc
% return 
%% INPUT PARAMETERS

theta0 = 1.5;
g      = 9.8;
L      = 1.0;


x0    = theta0;
v0    = 0;

dt    = 0.01; % TIME STEP FOR CALCULATION
tmin  = 0;
tmax  = 10;

dtplot= 0.01; % TIME STEP FOR PLOTTING
%% INITIALIZATION
tj = tmin;
tjplot = tmin;
xj = x0;
vj = v0;

xdata = [];
vdata = [];
tdata = [];
%% MAIN LOOP
while tj<=(tmax+dt)
    if tj>=tjplot
        xdata = [xdata; xj];
        vdata = [vdata; vj];
        tdata = [tdata; tj];
        tjplot = tjplot+dtplot;
    end
    [kx1,kv1] = Ftxv(g,L,xj,vj);
    [kx2,kv2] = Ftxv(g,L,xj+0.5*dt*kx1,vj+0.5*dt*kv1); 
    [kx3,kv3] = Ftxv(g,L,xj+0.5*dt*kx2,vj+0.5*dt*kv2); 
    [kx4,kv4] = Ftxv(g,L,xj+    dt*kx3,vj+    dt*kv3); 

    xjp = xj + (dt/6)*(kx1 + 2*kx2 + 2*kx3 + kx4);
    vjp = vj + (dt/6)*(kv1 + 2*kv2 + 2*kv3 + kv4);
    
    xj = xjp;
    vj = vjp;
    tj = tj+dt;
end

%% VISUALIZATION

figure();
plot(xdata,vdata,'-','LineWidth',2);
xlabel('x');
ylabel('v');
axis equal

figure();
plot(tdata,xdata,'-','LineWidth',2)
xlabel('t');
ylabel('x');
