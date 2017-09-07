/*******************************************************************************
* Copyright 2009-2017 Intel Corporation All Rights Reserved.
*
* The source code,  information  and material  ("Material") contained  herein is
* owned by Intel Corporation or its  suppliers or licensors,  and  title to such
* Material remains with Intel  Corporation or its  suppliers or  licensors.  The
* Material  contains  proprietary  information  of  Intel or  its suppliers  and
* licensors.  The Material is protected by  worldwide copyright  laws and treaty
* provisions.  No part  of  the  Material   may  be  used,  copied,  reproduced,
* modified, published,  uploaded, posted, transmitted,  distributed or disclosed
* in any way without Intel's prior express written permission.  No license under
* any patent,  copyright or other  intellectual property rights  in the Material
* is granted to  or  conferred  upon  you,  either   expressly,  by implication,
* inducement,  estoppel  or  otherwise.  Any  license   under such  intellectual
* property rights must be express and approved by Intel in writing.
*
* Unless otherwise agreed by Intel in writing,  you may not remove or alter this
* notice or  any  other  notice   embedded  in  Materials  by  Intel  or Intel's
* suppliers or licensors in any way.
*******************************************************************************/

/*
   DSYSV Example.
   ==============
 
   The program computes the solution to the system of linear equations
   with a real symmetric matrix A and multiple right-hand sides B,
   where A is the coefficient matrix:

    -5.86   3.99  -5.93  -2.82   7.69
     3.99   4.46   2.58   4.42   4.61
    -5.93   2.58  -8.52   8.57   7.69
    -2.82   4.42   8.57   3.72   8.07
     7.69   4.61   7.69   8.07   9.83

   and B is the right-hand side matrix:
 
     1.32  -6.33  -8.77
     2.22   1.69  -8.33
     0.12  -1.56   9.54
    -6.41  -9.49   9.56
     6.33  -3.67   7.48
 
   Description.
   ============
 
   The routine solves for X the real system of linear equations A*X = B,
   where A is an n-by-n symmetric matrix, the columns of matrix B are
   individual right-hand sides, and the columns of X are the corresponding
   solutions.

   The diagonal pivoting method is used to factor A as A = U*D*UT or
   A = L*D*LT , where U (or L) is a product of permutation and unit upper
   (lower) triangular matrices, and D is symmetric and block diagonal with
   1-by-1 and 2-by-2 diagonal blocks.

   The factored form of A is then used to solve the system of equations A*X = B.

   Example Program Results.
   ========================
 
 DSYSV Example Program Results

 Solution
   1.17   0.52  -0.86
  -0.71   1.05  -4.90
  -0.63  -0.52   0.99
  -0.33   0.43   1.22
   0.83  -1.22   1.96

 Details of factorization
  -5.86   0.00   0.00   0.00   0.00
  -0.68   7.18   0.00   0.00   0.00
   1.01  -0.20  -2.82   0.00   0.00
   0.48   0.35  11.93   4.21   0.00
  -1.31   1.37   0.02   0.16   6.22

 Pivot indices
      1      2     -4     -4      5
*/
#include <stdlib.h>
#include <stdio.h>
#include <mkl.h>

/* Auxiliary routines prototypes */
extern void print_matrix( char* desc, int m, int n, double* a, int lda );
extern void print_int_vector( char* desc, int n, int* a );

/* Parameters */
#define N 5
#define NRHS 3
#define LDA N
#define LDB N

/* Main program */
int main() {
	/* Locals */
	MKL_INT n = N, nrhs = NRHS, lda = LDA, ldb = LDB, info, lwork;
	double wkopt;
	double* work;
	/* Local arrays */
	MKL_INT ipiv[N];
	double a[LDA*N] = {
	   -5.86,  3.99, -5.93, -2.82,  7.69,
	    0.00,  4.46,  2.58,  4.42,  4.61,
	    0.00,  0.00, -8.52,  8.57,  7.69,
	    0.00,  0.00,  0.00,  3.72,  8.07,
	    0.00,  0.00,  0.00,  0.00,  9.83
	};
	double b[LDB*NRHS] = {
	    1.32,  2.22,  0.12, -6.41,  6.33,
	   -6.33,  1.69, -1.56, -9.49, -3.67,
	   -8.77, -8.33,  9.54,  9.56,  7.48
	};
	/* Executable statements */
	printf( " DSYSV Example Program Results\n" );
	/* Query and allocate the optimal workspace */
	lwork = -1;
	dsysv( "Lower", &n, &nrhs, a, &lda, ipiv, b, &ldb, &wkopt, &lwork, &info );
	lwork = (MKL_INT)wkopt;
	work = (double*)malloc( lwork*sizeof(double) );
	/* Solve the equations A*X = B */
	dsysv( "Lower", &n, &nrhs, a, &lda, ipiv, b, &ldb, work, &lwork, &info );
	/* Check for the exact singularity */
	if( info > 0 ) {
		printf( "The element of the diagonal factor " );
		printf( "D(%i,%i) is zero, so that D is singular;\n", info, info );
		printf( "the solution could not be computed.\n" );
		exit( 1 );
	}
	/* Print solution */
	print_matrix( "Solution", n, nrhs, b, ldb );
	/* Print details of factorization */
	print_matrix( "Details of factorization", n, n, a, lda );
	/* Print pivot indices */
	print_int_vector( "Pivot indices", n, ipiv );
	/* Free workspace */
	free( (void*)work );
	exit( 0 );
} /* End of DSYSV Example */

/* Auxiliary routine: printing a matrix */
void print_matrix( char* desc, int m, int n, double* a, int lda ) {
	int i, j;
	printf( "\n %s\n", desc );
	for( i = 0; i < m; i++ ) {
		for( j = 0; j < n; j++ ) printf( " %6.2f", a[i+j*lda] );
		printf( "\n" );
	}
}

/* Auxiliary routine: printing a vector of integers */
void print_int_vector( char* desc, int n, int* a ) {
	int j;
	printf( "\n %s\n", desc );
	for( j = 0; j < n; j++ ) printf( " %6i", a[j] );
	printf( "\n" );
}
