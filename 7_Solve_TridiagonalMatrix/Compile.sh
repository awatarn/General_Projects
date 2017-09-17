icc -O3 Solve_CyclicTridiagonalMatrix.c -o a.out -L /opt/intel/mkl/lib -lmkl_intel_ilp64 -lmkl_sequential -lmkl_core -lpthread -lm -DMKL_ILP64
