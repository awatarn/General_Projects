rm a.out
mpiifort helloworld.f90
mpirun -n 8 ./a.out
