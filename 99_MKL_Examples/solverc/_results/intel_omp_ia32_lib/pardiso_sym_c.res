
=== PARDISO: solving a symmetric indefinite system ===
1-based array indexing is turned ON
PARDISO double precision computation is turned ON
METIS algorithm at reorder step is turned ON
Scaling is turned ON


Summary: ( reordering phase )
================

Times:
======
Time spent in calculations of symmetric matrix portrait (fulladj): 0.000282 s
Time spent in reordering of the initial matrix (reorder)         : 0.000818 s
Time spent in symbolic factorization (symbfct)                   : 0.000369 s
Time spent in data preparations for factorization (parlist)      : 0.000087 s
Time spent in allocation of internal data structures (malloc)    : 0.000179 s
Time spent in additional calculations                            : 0.000028 s
Total time spent                                                 : 0.001763 s

Statistics:
===========
Parallel Direct Factorization is running on 2 OpenMP

< Linear system Ax = b >
             number of equations:           8
             number of non-zeros in A:      18
             number of non-zeros in A (%): 28.125000

             number of right-hand sides:    1

< Factors L and U >
             number of columns for each panel: 128
             number of independent subgraphs:  0
< Preprocessing with state of the art partitioning metis>
             number of supernodes:                    4
             size of largest supernode:               4
             number of non-zeros in L:                31
             number of non-zeros in U:                1
             number of non-zeros in L+U:              32

Reordering completed ... 
Number of nonzeros in factors = 32
Number of factorization MFLOPS = 0=== PARDISO is running in In-Core mode, because iparam(60)=0 ===

Percentage of computed non-zeros for LL^T factorization
 25 %  38 %  48 %  100 % 

=== PARDISO: solving a symmetric indefinite system ===
Single-level factorization algorithm is turned ON


Summary: ( factorization phase )
================

Times:
======
Time spent in copying matrix to internal data structure (A to LU): 0.000000 s
Time spent in factorization step (numfct)                        : 0.001339 s
Time spent in allocation of internal data structures (malloc)    : 0.000034 s
Time spent in additional calculations                            : 0.000002 s
Total time spent                                                 : 0.001375 s

Statistics:
===========
Parallel Direct Factorization is running on 2 OpenMP

< Linear system Ax = b >
             number of equations:           8
             number of non-zeros in A:      18
             number of non-zeros in A (%): 28.125000

             number of right-hand sides:    1

< Factors L and U >
             number of columns for each panel: 128
             number of independent subgraphs:  0
< Preprocessing with state of the art partitioning metis>
             number of supernodes:                    4
             size of largest supernode:               4
             number of non-zeros in L:                31
             number of non-zeros in U:                1
             number of non-zeros in L+U:              32
             gflop   for the numerical factorization: 0.000000

             gflop/s for the numerical factorization: 0.000055


Factorization completed ... 
=== PARDISO: solving a symmetric indefinite system ===


Summary: ( solution phase )
================

Times:
======
Time spent in direct solver at solve step (solve)                : 0.000488 s
Time spent in additional calculations                            : 0.000473 s
Total time spent                                                 : 0.000961 s

Statistics:
===========
Parallel Direct Factorization is running on 2 OpenMP

< Linear system Ax = b >
             number of equations:           8
             number of non-zeros in A:      18
             number of non-zeros in A (%): 28.125000

             number of right-hand sides:    1

< Factors L and U >
             number of columns for each panel: 128
             number of independent subgraphs:  0
< Preprocessing with state of the art partitioning metis>
             number of supernodes:                    4
             size of largest supernode:               4
             number of non-zeros in L:                31
             number of non-zeros in U:                1
             number of non-zeros in L+U:              32
             gflop   for the numerical factorization: 0.000000

             gflop/s for the numerical factorization: 0.000055


Solve completed ... 
The solution of the system is: 
 x [0] = -0.041860
 x [1] = -0.003413
 x [2] =  0.117250
 x [3] = -0.112640
 x [4] =  0.024172
 x [5] = -0.107633
 x [6] =  0.198720
 x [7] =  0.190383
