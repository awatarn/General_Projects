close all
clear
clc

%% 

global D LogRankNo


% filename = 'SET50-1-Day.xlsx';
filename = '../SET_EOD_1975_2018.xlsx';
ExcelData = xlsread(filename);

% First = 2029;
% Last = 4000;
First = 1;
Step = 1;
Last = 10725;
    
OpenP = ExcelData(First:Step:Last,2);
CloseP = ExcelData(First:Step:Last,5);
DateList = ExcelData(First:Step:Last,7); % 1:length(CloseP);
DiffP = OpenP - CloseP;

figure();
plot(DateList,OpenP,'b-',DateList,CloseP,'r-')
title('Open index and closure index');

% figure();
% hist(CloseP,20)


NData = length(DiffP);
% figure();
% hist(DiffP,60)

% pd = fitdist(DiffP,'Normal');

P = CloseP;
N = length(P);

DUnsorted = [];
DatePmax = [];
DatePmin = [];

for k = 2:N-1
    
    dayx = datetime(DateList(k),'ConvertFrom','excel');
    temp1 = sprintf('%s %5d %7.2f',dayx,k,P(k));
%     disp(temp1)

    if (P(k)>=P(k-1)) && (P(k)>P(k+1))  % Find the local maximum
        Pmax = P(k);                    % Local maximum
        dayx = datetime(DateList(k),'ConvertFrom','excel');
        temp2 = sprintf('%s Pmax(%5d) = %7.2f',dayx,k,Pmax);
%         disp(temp2)
        n = 1;
        LocalMinTrigger = 0;
        IncreasingTrigger = 0;
        while ((n+k<N-1) && (LocalMinTrigger == 0) && (IncreasingTrigger == 0))
            if (P(n+k) > Pmax)
                IncreasingTrigger = 1;
            end
            if (P(k+n)<P(k+n+1)) && (P(k+n) < P(k+n-1) && (IncreasingTrigger == 0))
                LocalMinTrigger = 1;    % Condition for local minimum is satisfy
                Pmin = P(k+n);          % Local minimum       
                Dkn = (Pmin - Pmax)/Pmax;   % Drawdown
                if Dkn < -0.15
                    disp(temp2)
                    dayx = datetime(DateList(k+n),'ConvertFrom','excel');
                    temp3 = sprintf('%s Pmin(%5d) = %7.2f @ n = %2d @ Dkn = %.4f',dayx,k+n,Pmin,n,Dkn);
                    disp(temp3)
                    
                    DatePmaxi = [DateList(k) Pmax];
                    DatePmax = [DatePmax; DatePmaxi];
                    
                    DatePmini = [DateList(k+n) Pmin];
                    DatePmin = [DatePmin; DatePmini];
                end
                DUnsorted = [DUnsorted Dkn];            % Drawdown (Unsorted data)
            end
            n = n + 1;
        end
    end
end

% Sort drawdown
D = sort(DUnsorted);                                    % Sorted drawdown
RankNo = 1:length(D);                                   % Rank number
LogRankNo = log(RankNo);                                % Take log(Rank No.)

% Plot Log(Rank number) vs Drawdown
figure(); 
plot(D,LogRankNo,'or')
title('Log(RankNo.) vs D');


% Plot histogram of drawdown
[y, centers] =hist(D,100);
figure();
plot(centers, y,'ro')
title('Histogram of drawdown');

% Fit drawdown
ObjectiveFunction = @Fitness_StretchedExpo;
nvars = 3;                  % Number of variables
LB    = [-100 0 -10];  % Lower bound
UB    = [100 30 10];  % Upper bound
ConstraintFunction = @simple_constraint;
options = optimoptions(@ga,'MutationFcn',@mutationadaptfeasible);
options = optimoptions(options,'PlotFcn',{@gaplotbestf,@gaplotmaxconstr}, ...
    'Display','iter');
[x,fval] = ga(ObjectiveFunction,nvars,[],[],[],[],LB,UB, ...
              ConstraintFunction,options);

%% 
temp1 = sprintf('fval  = %f',fval);
disp(temp1)
temp1 = sprintf('log(A)= %f',x(1));
disp(temp1)
temp1 = sprintf('b     = %f',x(2));
disp(temp1)
temp1 = sprintf('z     = %f',x(3));
disp(temp1)

%%
PredictedLogN = x(1) - x(2)*abs(D).^x(3);
figure(); 
plot(D,LogRankNo,'.r',D,PredictedLogN,'--b','LineWidth',2,'MarkerSize',12);
xlabel('Drawdown');
ylabel('Log(Rank number)');
%%
figure();
plot(DateList,CloseP,'k-')
hold on
plot(DatePmax(:,1),DatePmax(:,2),'^b','MarkerSize',10,'MarkerFaceColor','b','MarkerEdgeColor','b');
plot(DatePmin(:,1),DatePmin(:,2),'or','MarkerSize',10,'MarkerFaceColor','r','MarkerEdgeColor','r');
hold off
title('Closure vs Date number');
xlabel('Date number');
ylabel('Closure index');


