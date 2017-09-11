close all
clear
clc
%% https://www.cise.ufl.edu/research/sparse/matrices/list_by_dimension.html
% MatrixFilename = 'cryg10000.mat';
MatrixFilename = 'olm5000.mat';
load(MatrixFilename);
A = Problem.A;

% aa=ones(5,5).*0;
% 
% aa(1,1) = 1;
% aa(1,2) = -1;
% aa(1,4) = -3;
% aa(2,1) = -2;
% aa(2,2) = 5;
% aa(3,3) = 4;
% aa(3,4) = 6;
% aa(3,5) = 4;
% aa(4,1) = -4;
% aa(4,3) = 2;
% aa(4,4) = 7;
% aa(5,2) = 8;
% aa(5,5) = -5;
% 
% A = aa;


SizeA = size(A);
LenA = SizeA(1)*SizeA(2);

a=[]; % matrix element
i = 0;
ja = []; % column index (CSR3 format)
ia = []; % row index (CSR3 format)
jb = []; % row index (direct counting)
for n = 1:SizeA(1)
    foundZero = 0;
    for m = 1:SizeA(2)
        if A(n,m)~=0
            i = i+1;
            a(i) = A(n,m);
            ja(i) = m;
            jb(i) = n;
            if foundZero == 0
                foundZero = 1;
                ia(n) = i;
            end
        end
    end
end

ia(n+1) = i+1;

%% Convert to CSR3 format
% clc
% ia
% ja
% a

temp1 = sprintf('    /* Matrix data. */');
temp2 = sprintf('    MKL_INT n = %d;',SizeA(1));
temp3 = sprintf('    MKL_INT ia[%d] = {\n   ',length(ia));
temp31 = sprintf('');
for i = 1:length(ia)
    if i<length(ia)
        temp31 = sprintf('%s %3d,',temp31,ia(i)-1);
    else
        temp31 = sprintf('%s %3d};',temp31,ia(i)-1);
    end
    if mod(length(temp31),70)>60
        temp31 = sprintf('%s\n   ',temp31);
    end        
end
temp4 = sprintf('%s%s',temp3,temp31);

temp5 = sprintf('    MKL_INT ja[%d] = {\n   ',length(ja));
temp51 = sprintf('');
for i = 1:length(ja)
    if i<length(ja)
        temp51 = sprintf('%s %3d,',temp51,ja(i)-1);
    else
        temp51 = sprintf('%s %3d};',temp51,ja(i)-1);
    end
    if mod(length(temp51),70)>64
        temp51 = sprintf('%s\n   ',temp51);
    end    
end
temp6 = sprintf('%s%s',temp5,temp51);

temp7 = sprintf('    double a[%d] = {\n   ',length(a));
temp71 = sprintf('');
for i = 1:length(a)
    if i<length(a)
        temp71 = sprintf('%s %10.6e,',temp71,a(i));
    else
        temp71 = sprintf('%s %10.6e};',temp71,a(i));
    end
    if mod(length(temp71),70)>59
        temp71 = sprintf('%s\n   ',temp71);
    end
end
temp8 = sprintf('%s%s',temp7,temp71);

temp9 = sprintf('#define NN %d',SizeA(1));

clc
disp(temp9)
disp(temp1)
disp(temp2)
disp(temp4)
disp(temp6)
disp(temp8)
x = inv(A)*ones(SizeA(1),1);
x(length(x)-5:length(x))


% return
% temp10 = sprintf('');
% for i=1:length(a)
%     temp10 = sprintf('%s\nA[%d][%d] = %15.5e;',temp10,jb(i)-1,ja(i)-1,a(i));
% end
% clc
% disp(temp10)

