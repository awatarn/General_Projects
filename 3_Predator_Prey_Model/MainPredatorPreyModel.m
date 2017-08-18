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
% alpha = 2/3;
% beta  = 4/3;
% gamma = 1;
% delta = 1;

alpha = 0.4;
beta  = 1.0;
gamma = 1.0;
delta = 1.0;

% x0    = gamma/delta*(1+0.01);
% y0    = alpha/beta*(1+0.01);

x0 = 1.01;
y0 = 0.404;
dt    = 0.00001;
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
%         xdata = [xdata; (xj - gamma/delta)];
%         ydata = [ydata; (yj - alpha/beta)];
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

 
%% DETERMINE PERIOD (TIME INTERVAL BETWEEN CONSECUTIVE PEAKS)
[xpks, xlocs] = findpeaks(xdata);
Tx = tdata(xlocs(3)) - tdata(xlocs(2));
Txcal = 2*pi/sqrt(alpha*gamma);

[ypks, ylocs] = findpeaks(xdata);
Ty = tdata(ylocs(3)) - tdata(ylocs(2));

temp1 = sprintf('alpha     = %.6f',alpha);
disp(temp1)
temp1 = sprintf('beta      = %.6f',beta);
disp(temp1)
temp1 = sprintf('gamma     = %.6f',gamma);
disp(temp1)
temp1 = sprintf('delta     = %.6f',delta);
disp(temp1)

temp1 = sprintf('x_infty   = %.4f    y_infty   = %.4f',gamma/delta,alpha/beta);
disp(temp1)
temp1 = sprintf('x_initial = %.4f    y_initial = %.4f',x0,y0);
disp(temp1)
temp1 = sprintf('T_sim     = %.4f s  T_theory  = %.4f s',Tx,Txcal);
disp(temp1)

AAA = [alpha beta gamma delta Tx Txcal];


return 






















%% INITIALIZE THE MOVIE MODULE
Time_i = tdata(1:1:length(tdata)); 
index=Time_i;
noofindex = length(Time_i) - mod(length(Time_i),4);
nFrames = 100; %noofindex;
mov(1:nFrames) = struct('cdata', [],...
                        'colormap', []);
fullscreen = get(0,'ScreenSize');   
fh = figure('OuterPosition',[100 50 1200 550],'Resize','off','ToolBar','none');  
set(fh, 'color', 'white');
k = 0;
iA = 1;
sprintf('index(iA) = %.8f',index(iA))

% % FNTSZ = 24; % axis font size
set(gcf,'DefaultAxesFontSize', 20)
for i=1:1:nFrames
    k=k+1
    
    h1=subplot(121)
    hold on
    plot(tdata(k),xdata(k),'r.','LineWidth',2);    
    plot(tdata(k),ydata(k),'b.','LineWidth',2);
    hold off
    legend('x (prey)','y (predator)');
    xlabel('Time');
    ylabel('x or y');
    xlim([0 30]);
    ylim([0 2]);
    
    h2=subplot(122)
    hold on
    plot(xdata(k),ydata(k),'k.','LineWidth',2);
    hold off
    xlabel('x (prey)');
    ylabel('y (predator)');
    xlim([0 2]);
    ylim([0 2]);
    
    % add movie frame
    if k==1
        set(gcf,'nextplot','replacechildren');
        h1pos=get(h1,'position');
        h2pos=get(h2,'position');
      
    else
        set(h1,'position',h1pos);
        set(h2,'position',h2pos);
    end
        
%     pause(1);
    mov(k) = getframe(gcf); 
end

%% export movie
Switch_ToSave_Movie = 0;
if Switch_ToSave_Movie == 1
    temp1= sprintf('Movie1.avi');
    % FOR R2016B OR LATER    
    v = VideoWriter(temp1);
    v.FrameRate=4;
    open(v);
    writeVideo(v,mov)
    close(v)
    
    % FOR 2015A OR OLDER
%     movie2avi(mov,temp1,'compression','None','fps',4);
end
