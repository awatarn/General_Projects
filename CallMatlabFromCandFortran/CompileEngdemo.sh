source /home/glass/.profile
#echo "LD_LIBRARY_PATH>> "
#echo $LD_LIBRARY_PATH
#echo "PATH>> "
#echo $PATH
gcc engdemo.c -o engdemo -I/usr/local/MATLAB/MATLAB_Production_Server/R2013a/extern/include/ -L /usr/local/MATLAB/MATLAB_Production_Server/R2013a/bin/glnxa64 -leng -lmx -lmex

