close all
clear
clc


filename = 'SET50-1-Day.xlsx';
ExcelData = xlsread(filename);

First = 2029;
Last = 4000;
OpenPrice = ExcelData(First:Last,2);


    
OpenP = ExcelData(First:Last,2);
CloseP = ExcelData(First:Last,5);
DateList = 1:length(CloseP);
DiffP = OpenP - CloseP;

figure();
plot(DateList,OpenP,'b-',DateList,CloseP,'r-')

% figure();
% hist(CloseP,20)


NData = length(DiffP);
% figure();
% hist(DiffP,60)

% pd = fitdist(DiffP,'Normal');

P = CloseP;
N = length(P);

D = [];


for k = 2:N-1
    %disp(k)
    if ((P(k)>=P(k-1)) && (P(k)>P(k+1)))
        Pmax = P(k);
        CheckBelow = 1;
        for n = 1:N-1-k
            if P(k+n) > P(k)
                CheckBelow = 0;
            end
            if ((P(k+n) < P(k)) && (CheckBelow == 1))
                if ( (P(k+n)<P(k+n-1)) && (P(k+n)<=P(k+n+1)))
                    Pmin = P(k+n);
                    Dkn = (Pmin - Pmax)/Pmax;
                    if Dkn <= -0.60
                        temp0 = sprintf('k=%d ,P(k)=%.2f, n=%d, P(k+n)=%.2f',k,P(k),n,P(k+n));
                        disp(temp0)
                    end
                    D = [D Dkn];
                                        
                end
            end
        end
    end
end

d1 = D;
D = sort(d1);
RankNo = 1:length(D);
z = log(RankNo);

figure(); 
plot(D,z,'or')
title('Log(RankNo. vs D)');


[y, centers] =hist(D,100);
% figure();
% plot(centers, y,'ro')


% figure()
% plot(y,'o')

% z = log(fliplr(y));



r = D; 
% fun = @(x) x(1) - x(2)*abs(r).^0.75 + x(3) -z;
fun = @(x) x(1) - x(2)*abs(r).^x(3) -z;
d0 = [100,10,0.001];
x = lsqnonlin(fun,d0)


plot(r,z,'ro',r,fun(x)+z,'bo');



