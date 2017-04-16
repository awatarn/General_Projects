close all
clear
clc
%%
alpha = -1;
beta  = 1;
delta = 0.1;
gamma = 0.1;
omega = 1.4;
x0    = 0;
v0    = 0;

dt    = 0.1;
tmin  = 0;
tmax  = 50;

dtplot= 0.1;
%%
T = 2*pi/omega
tj = tmin;
tjplot = tmin;
xj = x0;
vj = v0;


xdata = [];
vdata = [];
tdata = [];
%%
while tj<=(tmax+dt)
    if tj>=tjplot
        xdata = [xdata; xj];
        vdata = [vdata; vj];
        tdata = [tdata; tj];
        tjplot = tjplot+dt;
    end
    xjp = xj + dt*vj;
    temp1 = vj - dt*(alpha*xj + beta*xj*xj*xj - gamma*cos(omega*tj));
    vjp = temp1/(1 + dt*delta);
    
    xj = xjp;
    vj = vjp;
    tj = tj+dt;
end

%%

figure();
plot(xdata,vdata,'-');

figure();
plot(tdata,xdata,'.')
