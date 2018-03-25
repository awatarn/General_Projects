close all
clear
clc
%% SIMPLE PLOT
x=-2*pi:0.01:2*pi;
y1=sin(x);
y2=sin(x).*sin(x);
y3=sin(x).^3;

figure()
plot(x,y1,'-r','LineWidth',2)
hold on
plot(x,y2,'-b','LineWidth',2)
plot(x,y3,'k--','LineWidth',2)
hold off
legend('sin(x)','sin(x)^2','sin(x)^3');
xlabel('x');
ylabel('y');

%% IF STATEMENT
a = randi(100, 1);

if a < 30
    disp('small')
elseif a < 80
    disp('medium')
else
    disp('large')
end

%% FOR LOOP
sum = 0;

for i=1:1:100
    sum = sum + i;
end

sum


%% WHILE LOOP

sum = 0;
i = 0;

while i<=100
    sum = sum + i;
    i = i + 1;
end

sum


%%













