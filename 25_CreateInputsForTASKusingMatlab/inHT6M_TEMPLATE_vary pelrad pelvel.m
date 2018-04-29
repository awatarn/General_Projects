close all
clear
clc
%%
Foldername_template = 'inHT6M_TEMPLATE';
Output_prefix = 'inHT6Ms378'; % PELRAD = 3.78e-4, vary PELVEL
RunMe_name = sprintf('RunMe_%s.sh',Output_prefix);

RunMe_output = sprintf('%s',RunMe_name);
fileOUT2 = fopen(RunMe_output,'w');

AAList = 3.78e-4; % PELRAD
BBList = 100:100:1000; % PELVEL

index = 0;
for index = 1:1:length(BBList)
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
        AAAAAIndex = strfind(tline,'AAAAA'); % PELRAD
        if AAAAAIndex>0
%             disp(tline)
            temp1 = tline(1:AAAAAIndex-1);
            tline = sprintf('%s%.4e',temp1,AAList);
%             disp(tline)
        end
        
        BBBBBIndex = strfind(tline,'BBBBB'); % PELRAD
        if BBBBBIndex>0
%             disp(tline)
            temp1 = tline(1:BBBBBIndex-1);
            tline = sprintf('%s%.4e',temp1,BBList(index));
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
