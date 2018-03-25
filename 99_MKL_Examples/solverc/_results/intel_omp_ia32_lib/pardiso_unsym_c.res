
=== PARDISO: solving a real nonsymmetric system ===
1-based array indexing is turned ON
PARDISO double precision computation is turned ON
METIS algorithm at reorder step is turned ON
Scaling is turned ON
Matching is turned ON


Summary: ( reordering phase )
================

Times:
======
Time spent in calculations of symmetric matrix portrait (fulladj): 0.000009 s
Time spent in reordering of the initial matrix (reorder)         : 0.000610 s
Time spent in symbolic factorization (symbfct)                   : 0.000403 s
Time spent in data preparations for factorization (parlist)      : 0.000104 s
Time spent in allocation of internal data structures (malloc)    : 0.000529 s
Time spent in additional calculations                            : 0.000039 s
Total time spent                                                 : 0.001694 s

Statistics:
===========
Parallel Direct Factorization is running on 2 OpenMP

< Linear system Ax = b >
             number of equations:           5
             number of non-zeros in A:      13
             number of non-zeros in A (%): 52.000000

             number of right-hand sides:    1

< Factors L and U >
             number of columns for each panel: 128
             number of independent subgraphs:  0
< Preprocessing with state of the art partitioning metis>
             number of supernodes:                    2
             size of largest supernode:               4
             number of non-zeros in L:                19
             number of non-zeros in U:                2
             number of non-zeros in L+U:              21

Reordering completed ... 
Number of nonzeros in factors = 21
Number of factorization MFLOPS = 0=== PARDISO is running in In-Core mode, because iparam(60)=0 ===

Percentage of computed non-zeros for LL^T factorization
 15 %  100 % 

=== PARDISO: solving a real nonsymmetric system ===
Single-level factorization algorithm is turned ON


Summary: ( factorization phase )
================

Times:
======
Time spent in copying matrix to internal data structure (A to LU): 0.000000 s
Time spent in factorization step (numfct)                        : 0.001031 s
Time spent in allocation of internal data structures (malloc)    : 0.000028 s
Time spent in additional calculations                            : 0.000002 s
Total time spent                                                 : 0.001061 s

Statistics:
===========
Parallel Direct Factorization is running on 2 OpenMP

< Linear system Ax = b >
             number of equations:           5
             number of non-zeros in A:      13
             number of non-zeros in A (%): 52.000000

             number of right-hand sides:    1

< Factors L and U >
             number of columns for each panel: 128
             number of independent subgraphs:  0
< Preprocessing with state of the art partitioning metis>
             number of supernodes:                    2
             size of largest supernode:               4
             number of non-zeros in L:                19
             number of non-zeros in U:                2
             number of non-zeros in L+U:              21
             gflop   for the numerical factorization: 0.000000

             gflop/s for the numerical factorization: 0.000048


Factorization completed ... 

Solving non-transposed system...

=== PARDISO: solving a real nonsymmetric system ===


Summary: ( solution phase )
================

Times:
======
Time spent in direct solver at solve step (solve)                : 0.000572 s
Time spent in additional calculations                            : 0.000552 s
Total time spent                                                 : 0.001124 s

Statistics:
===========
Parallel Direct Factorization is running on 2 OpenMP

< Linear system Ax = b >
             number of equations:           5
             number of non-zeros in A:      13
             number of non-zeros in A (%): 52.000000

             number of right-hand sides:    1

< Factors L and U >
             number of columns for each panel: 128
             number of independent subgraphs:  0
< Preprocessing with state of the art partitioning metis>
             number of supernodes:                    2
             size of largest supernode:               4
             number of non-zeros in L:                19
             number of non-zeros in U:                2
             number of non-zeros in L+U:              21
             gflop   for the numerical factorization: 0.000000

             gflop/s for the numerical factorization: 0.000048


The solution of the system is: 
 x [0] = -0.522321
 x [1] = -0.008929
 x [2] =  1.220982
 x [3] = -0.504464
 x [4] = -0.214286

Relative residual = 2.275280e-16

Solving conjugate transposed system...

=== PARDISO: solving a real nonsymmetric system ===


Summary: ( solution phase )
================

Times:
======
Time spent in direct solver at solve step (solve)                : 0.000008 s
Time spent in additional calculations                            : 0.000211 s
Total time spent                                                 : 0.000219 s

Statistics:
===========
Parallel Direct Factorization is running on 2 OpenMP

< Linear system Ax = b > < transpose >
             number of equations:           5
             number of non-zeros in A:      13
             number of non-zeros in A (%): 52.000000

             number of right-hand sides:    1

< Factors L and U >
             number of columns for each panel: 128
             number of independent subgraphs:  0
< Preprocessing with state of the art partitioning metis>
             number of supernodes:                    2
             size of largest supernode:               4
             number of non-zeros in L:                19
             number of non-zeros in U:                2
             number of non-zeros in L+U:              21
             gflop   for the numerical factorization: 0.000000

             gflop/s for the numerical factorization: 0.000048


The solution of the system is: 
 x [0] = -0.220238
 x [1] = -0.029762
 x [2] =  0.395089
 x [3] = -0.290179
 x [4] =  0.116071

Relative residual = 1.216188e-16

Solving transposed system...

=== PARDISO: solving a real nonsymmetric system ===


Summary: ( solution phase )
================

Times:
======
Time spent in direct solver at solve step (solve)                : 0.000004 s
Time spent in additional calculations                            : 0.000018 s
Total time spent                                                 : 0.000022 s

Statistics:
===========
Parallel Direct Factorization is running on 2 OpenMP

< Linear system Ax = b > < transpose >
             number of equations:           5
             number of non-zeros in A:      13
             number of non-zeros in A (%): 52.000000

             number of right-hand sides:    1

< Factors L and U >
             number of columns for each panel: 128
             number of independent subgraphs:  0
< Preprocessing with state of the art partitioning metis>
             number of supernodes:                    2
             size of largest supernode:               4
             number of non-zeros in L:                19
             number of non-zeros in U:                2
             number of non-zeros in L+U:              21
             gflop   for the numerical factorization: 0.000000

             gflop/s for the numerical factorization: 0.000048


The solution of the system is: 
 x [0] = -0.220238
 x [1] = -0.029762
 x [2] =  0.395089
 x [3] = -0.290179
 x [4] =  0.116071

Relative residual = 1.216188e-16