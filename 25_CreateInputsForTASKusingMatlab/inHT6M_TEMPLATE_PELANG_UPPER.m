close all
clear
clc
%%
Foldername_template = 'inHT6M_TEMPLATE';
Output_prefix = 'inHT6Ma302'; % PELRAD = 3e-4, vary RDEBUT
RunMe_name = sprintf('RunMe_%s.sh',Output_prefix);

RunMe_output = sprintf('%s',RunMe_name);
fileOUT2 = fopen(RunMe_output,'w');

RR = 0.65;
aa = 0.40;
CCList = 0:pi/9:pi;
AAList = RR + aa*cos(CCList); % PELRDEB
BBList = aa*sin(CCList); % PELZDEB

index = 0;
for index = 1:1:length(CCList)
    if index<10
        Folder_i = sprintf('%s0%d',Output_prefix,index);
    elseif index>=10
        Folder_i = sprintf('%s%d',Output_prefix,index);
    end    
    disp(Folder_i)
    mkdir(Folder_i)
    
    % COPY FILES
    temp1 = sprintf('%s/*',Foldername_template);    
    copyfile(temp1,Folder_i);
    
    % READ TR2.INHT6M
    tr2_template = sprintf('%s/tr2.inHT6M_TEMPLATE',Foldername_template);
    tr2_output   = sprintf('%s/tr2.inHT6M',Folder_i);
    
    
    fileIN = fopen(tr2_template,'r');
    fileOUT = fopen(tr2_output,'w');
    
    while ~feof(fileIN)
        tline = fgetl(fileIN);        
%         disp(tline);
        AAAAAIndex = strfind(tline,'AAAAA'); % PELRDEB
        if AAAAAIndex>0
%             disp(tline)
            temp1 = tline(1:AAAAAIndex-1);
            tline = sprintf('%s%.4e',temp1,AAList(index));
%             disp(tline)
        end
        
        BBBBBIndex = strfind(tline,'BBBBB'); % PELZDEB
        if BBBBBIndex>0
%             disp(tline)
            temp1 = tline(1:BBBBBIndex-1);
            tline = sprintf('%s%.4e',temp1,BBList(index));
%             disp(tline)
        end        
        
        CCCCCIndex = strfind(tline,'CCCCC'); % PELZDEB
        if CCCCCIndex>0
%             disp(tline)
            temp1 = tline(1:CCCCCIndex-1);
            tline = sprintf('%s%.4e',temp1,CCList(index)-pi);
%             disp(tline)
        end
        
        fprintf(fileOUT,'%s\n',tline);
    end    
    
    % WRITE COMMAND TO RunMe_name file
    fprintf(fileOUT2,'cd %s\n',Folder_i);
    fprintf(fileOUT2,'chmod +x *.sh\n');
    fprintf(fileOUT2,'nohup ./RunMe.sh &\n');
    fprintf(fileOUT2,'cd ..\n\n');
    
    
    % close files
    fclose(fileIN);
    fclose(fileOUT);
    
    
end
fclose(fileOUT2);
