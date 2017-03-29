%% Reading2DDataFromTASK
%  Apiwat wrote this file on 26 March 2017
function [TimeMatch,rho,Y] = Reading2DDataFromTASK(DirectoryContainingData,File2dName,DepVarLabel,TimeQuery,Option)

%%
% clear 
% close all
% clc
%% Input
disp('Reading2DDataFromTASK Status: Initialized')
disp(' ')

% DirectoryContainingData = sprintf('/Users/vitreloy/git2017/TestScripts/1_Read2DUfileData');
% File2dName = sprintf('data_RT1.txt');
% DepVarLabel = 'TE';

FullPathFile2D = sprintf('%s/%s',DirectoryContainingData,File2dName);

% TimeQuery = 99;
% Option = 1; 

% temptext1=sprintf('Reading data from:\n%s',FullPathFile2D);
% temptext2=sprintf('Dependent varaible: %s',DepVarLabel);
% 
% disp(temptext1)
% disp(temptext2)
%% Internal variables
isDepVarLabel = 0;

%% Reading data
fid1 = fopen(FullPathFile2D,'r');
lineNo=0;
isEndOfData = 0;
isNUMLine = 0;

NoLineRadius = 0;
NoLineTime = 0;

HeaderLine = 3;
LineNo = 0; % Line number from beginning (1,2,3,...)
LineNoFirstData = 99999999999999999999;
LineNoTimeQuery = 0;

rho = [];
time = [];
Y = [];

while isEndOfData~=1
    tline = fgetl(fid1);
    LineNo = LineNo + 1;
%     disp(tline);
    lineNo=lineNo+1;
    
    kDepVarLabel=strfind(tline,DepVarLabel);
    if (isempty(kDepVarLabel)~= 1) && (isDepVarLabel == 0)
        if (Option == 1)
            disp('Actual reading variable:')
            disp(tline)
        end
        isDepVarLabel = 1;
    end
    isItNUMLine = strfind(tline,'NUM');
    if (isDepVarLabel == 1) && (isNUMLine == 0) && (isempty(isItNUMLine)~=1)
        SizeR = str2num(tline(5:13));
        SizeT = str2num(tline(13:length(tline)));
        isNUMLine = 1;
        
        NoLineRadius = ceil(SizeR/5); % # of lines of radial coordinates
        NoLineTime = ceil(SizeT/5);
        
        LineNoFirstData = HeaderLine+NoLineRadius+NoLineTime+1;
    end
    
    if (LineNo>=HeaderLine+1) && (NoLineRadius>0) && (LineNo<=HeaderLine+NoLineRadius)
%         disp(LineNo);
%         disp(tline);
        rho_temp1 = str2num(tline);
        rho = [rho rho_temp1];
        
    end
    
    if (LineNo>HeaderLine+NoLineRadius) && (NoLineTime>0) && (LineNo<=(HeaderLine+NoLineRadius+NoLineTime))
%         disp(LineNo)
%         disp(tline)
        time_temp1 = str2num(tline);
        time = [time time_temp1];
    end
    
    if (LineNo==LineNoFirstData-1)
        time;
        a = find(time<=TimeQuery);
        PosTimeQuery = max(a);
        TimeMatch = time(PosTimeQuery);
        LineNoTimeQuery = LineNoFirstData + (PosTimeQuery-1)*(SizeR/5);
    end
    
    if (LineNo>=LineNoTimeQuery) && (LineNoTimeQuery>0) && (LineNo<=(LineNoTimeQuery+NoLineRadius-1))
%         disp(tline)
        temp1 = str2num(tline);
        Y = [Y temp1];
    end
    
    if (LineNo>(LineNoTimeQuery+NoLineRadius-1)) && (LineNoTimeQuery>0)
        size(Y);
        size(rho);
        TimeMatch;
        
    end
    
    if (LineNo>=(LineNoTimeQuery+NoLineRadius)) && (LineNoTimeQuery>0)        
        if (Option >= 1) % Show summary
            temptext1=sprintf('Reading data from:\n%s',FullPathFile2D);
            temptext2=sprintf('Dependent varaible: %s\n',DepVarLabel);
            disp(temptext1)
            disp(temptext2)
            temptext3 = sprintf('Time requested     : %.4f s',TimeQuery);
            temptext4 = sprintf('Actual-return time : %.4f s\n',TimeMatch);
            disp(temptext3)
            disp(temptext4)
            
        end
        
        if (Option >= 2) % plot graph
            figure();
            temp1 = sprintf('Time %.4f s',TimeMatch);
            plot(rho,Y,'o')
            title(temp1)
            ylabel(DepVarLabel);
            xlabel('rho');
        end
        
        break
    end
end
fclose(fid1);
disp('Reading2DDataFromTASK Status: Done')

end

