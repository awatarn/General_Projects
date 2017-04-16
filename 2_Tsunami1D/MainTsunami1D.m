%% NUMERICAL SIMULATION OF TSUNAMI WAVE IN 1D
%  A. WISITSORASAK CREATED ON 16 APR 2017
%
%  THE SHALLOW WATER EQUATION IN 1D ARE
%  \partial \eta/\partial t + \partial M/\partial x = 0
%  \partial M/\partial t + g D + \partial \eta/\partial x = 0
%  \eta IS HEIGHT OF THE WATER SURFACE FROM THE UNDISTURBED WATER SURFACE.
%  M IS A QUANTITY DEFINED AS PRODUCT OF DEPTH AVERAGE VELOCITY AND WATER
%  VELOCITY IN THE +X DIRECTION.
%  D IS THE TOTAL THICKNESS OF WATER AT POTITION X. NOTE THAT D = h + \eta,
%  h IS DEPTH OF THE BASIN, MEASURE FROM THE UNDISTURBED WATER SURFACE
%  (DOWNWARD IS POSITIVE). 
%  INITIAL CONDITION: WATER WAS AT REST AT t = 0. M = 0 AND \eta(t=0) IS
%  GIVEN.
%  BOUNDARY CONDITIONS: AT X = 0 AND X = L: M = 0, DIVERGENCE FREE FOR
%  \eta.
%  NUMERICAL METHOD: SIMPLE EXPLICIT FINITE DIFFERENCE METHOD.
%  REFERENCES: "NUMERICAL SIMULATION OF WAVE PROPAGATION USING THE SHALLOW
%  WATER EQUATIONS" JUMBO PARK, HARVEY MUDD COLLEDGE, 26 APRIL 2007. 
%%
close all
clear
clc
%% INPUT PARAMETERS
xmin = 0;
xmax = 100;
nx   = 8*xmax;
dt   = 1/6000;
tmax = 3.0;
dtplot = 0.01;

%% INITIALIZATION
x  = linspace(xmin,xmax,nx);
xa = x.*0; 
xb = xa;
na  = xa;
nb  = xa;
Ma  = xa;
Mb  = xa;
h   = xa;
D   = xa;

ndata = [];

tiplot = 0;
dx = (xmax - xmin)/nx;

g = 9.81;
h = 50-45*tanh((x-70)/8); % height of the bottom of the ocean as measure 
                          % from the unperturbed surface.
% figure()
% plot(x,-h,'-')

na = 0.5*exp(-((x-20).^2)/8);
Ma = 100*na;
% figure();
% plot(x,na,'r-');
% hold on
% plot(x,0.1*Ma,'b-')
% hold off
%% MAIN LOOP
ti = 0;
while ti<=(tmax+dt)
    if ti>=tiplot
        ndata = [ndata; na];
        tiplot = tiplot+dtplot;
    end
    
    for xi=1:nx
        if xi==1
            nb(xi) = na(xi) - (0.5*dt/dx)*(Ma(xi+1)-0);
            Mb(xi) = 0;
        elseif ((xi>1) && (xi<nx))
            nb(xi) = na(xi) - (0.5*dt/dx)*(Ma(xi+1)-Ma(xi-1));
            Mb(xi) = Ma(xi) - g*(h(xi)+na(xi))*(0.5*dt/dx)*(na(xi+1)-na(xi-1));
        elseif xi==nx
            nb(xi) = na(xi) - (0.5*dt/dx)*(0-Ma(xi-1));
            Mb(xi) = 0;
        end
    end
    na = nb;
    Ma = Mb;
    nb = 0;
    Mb = 0;
    ti = ti+dt;
end

%% VISUALIZATION
figure();
imagesc(ndata)

figure();
plot(x,-h/50,'-k')
hold on
plot(x,ndata(1,:),'r-');
plot(x,na,'b-');
hold off

% plot(x,na,'o')