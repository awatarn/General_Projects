function ydiff = Ftheta(x)

global t0 tmax ExcelData bhat 

% x = [37691 0.151083 0 0];

tc    = x(1);
phi   = x(2);
omega = x(3);
z     = x(4);

%% Form vector yi, ti

tempy = find(ExcelData(:,7)>=t0);
First = tempy(1);

if tc<=tmax
    tfinal = tmax; %tc-1;
else
    tfinal = tmax;
end
% tfinal = tc-1;
tempz = find(ExcelData(:,7)>=tfinal);
Last  = tempz(1);
% OpenPrice = ExcelData(First:Last,2);

CloseP = ExcelData(First:Last,5);
DateList = ExcelData(First:Last,7);

yi = log10(CloseP);
ti = DateList;

%% Define f(t_i), g(t_i)
fti = abs(tc - ti).^z;
gti = cos(omega*log(abs(tc - ti)) + phi);


%% Form matrix X
XMat = ones(length(yi),3)*0;
for i = 1:length(yi)
    XMat(i,1) = 1;
    XMat(i,2) = fti(i);
    XMat(i,3) = gti(i);
end
XMatT = transpose(XMat);
% cond(XMatT)
temp1 = XMatT*XMat;
bhat  = (temp1\XMatT)*yi;

%% Compute the predicted y, yhat = A + B*f(t) + C*g(t)
A = bhat(1);
B = bhat(2);
C = bhat(3);
yhat = A + B*fti + C*gti;

% figure();
% plot(ti,yi,'ro',ti,yhat,'b-');

%% Form F_theta, the difference between the actual data and the predicted data
%  Equation 35

delta = yi - yhat;
ydiff = transpose(delta)*delta;


%%
