%% URANIUM DECAY
%  A. WISITOSRASAK CRREATED ON 20 APR 2017
%
%% CLEAR
clear
close all
clc

%% INPUT PARAMETERS
N0 = 100;
k  = 0.1;

tmin = 0;
tmax = 100; 
dt   = 0.01;

dtplot = 0.1;

%% INITIALIZATION
Nj = N0;
tj = tmin;
tjplot = tmin;

Ndata = [];
tdata = [];

%% MAIN LOOP
while tj<=(tmax+dt)
    if tj>=tjplot
        Ndata = [Ndata; Nj];
        tdata = [tdata; tj];
        
        tjplot = tjplot + dtplot;
    end
    Nj1 = Nj - dt*k*Nj;
    
    Nj = Nj1;
    tj = tj+dt;
end

%% VISUALIZATION

figure();
plot(tdata,Ndata,'-ro');
xlabel('time');
ylabel('N');

%% %% %% CREATE A MOVIE FILE FOR ANIMATION %% %% %%

%% INITIALIZE THE MOVIE MODULE
index = 1:10:length(tdata);                         % INDEX OF DATA

Time_i = tdata(index);                              % TIME INDEX
noofindex = length(Time_i) - mod(length(Time_i),4);
nFrames = 100; %noofindex                                              % TOTAL NUMBER OF FRAMES (THIS # MUST BE DIVISBIBLE BY # FRAME RATE PER SECOND)
mov(1:nFrames) = struct('cdata', [],...
                        'colormap', []);
fullscreen = get(0,'ScreenSize');   
fh = figure('OuterPosition',[100 50 1200 550],'Resize','off','ToolBar','none');  % SIZE AND POSITION OF FIGURE [X Y WIDTH HEIGHT]
set(fh, 'color', 'white');
iA = 1;
sprintf('index(iA) = %.8f',index(iA))

set(gcf,'DefaultAxesFontSize', 20);                             % AXIS FONT SIZE
k = 0;
for i=1:1:nFrames
    k=k+1
    
    m=index(k)
    h1=subplot(121)                                             % SUBPLOT 1
    hold on
    plot(tdata(m),Ndata(m),'r.','LineWidth',2);    
    plot(tdata(m),N0-Ndata(m),'b.','LineWidth',2);
    hold off
    legend('Remained particles','Lost particles');
    xlabel('Time');
    ylabel('Number of particles');
    xlim([tmin tmax]);
    ylim([0 N0]);
    
    h2=subplot(122)                                             % SUBPLOT 2
    hold on
    plot(tdata(m),Ndata(m),'r.','LineWidth',2);    
    plot(tdata(m),N0-Ndata(m),'b.','LineWidth',2);
    hold off
    legend('Remained particles','Lost particles');
    xlabel('Time');
    ylabel('Number of particles');
    xlim([tmin tmax]);
    ylim([0 N0]);
    
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
Switch_ToSave_Movie = 1;
if Switch_ToSave_Movie == 1
    temp1= sprintf('B_Uranium_movie.avi');
    % FOR R2016B OR LATER    
    v = VideoWriter(temp1);
    v.FrameRate=4;
    open(v);
    writeVideo(v,mov)
    close(v)
    
    % FOR 2015A OR OLDER
%     movie2avi(mov,temp1,'compression','None','fps',4);
end

