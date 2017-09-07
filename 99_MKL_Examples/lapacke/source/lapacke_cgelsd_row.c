/*******************************************************************************
* Copyright 2010-2017 Intel Corporation All Rights Reserved.
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
   LAPACKE_cgelsd Example.
   =======================

   Program computes the minimum norm-solution to a complex linear least squares
   problem using the singular value decomposition of A,
   where A is the coefficient matrix:

   (  4.55, -0.32) ( -4.36, -4.76) (  3.99, -6.84) (  8.03, -6.47)
   (  8.87, -3.11) (  0.02,  8.43) (  5.43, -9.30) (  2.28,  8.94)
   ( -0.74,  1.16) (  3.80, -6.12) ( -7.24,  0.72) (  2.21,  9.52)

   and B is the right-hand side matrix:

   ( -8.25,  7.98) (  2.91, -8.81)
   ( -5.04,  3.33) (  6.19,  0.19)
   (  7.98, -4.38) ( -5.96,  7.18)

   Description.
   ============

   The routine computes the minimum-norm solution to a complex linear least
   squares problem: minimize ||b - A*x|| using the singular value
   decomposition (SVD) of A. A is an m-by-n matrix which may be rank-deficient.

   Several right hand side vectors b and solution vectors x can be handled
   in a single call; they are stored as the columns of the m-by-nrhs right
   hand side matrix B and the n-by-nrhs solution matrix X.

   The effective rank of A is determined by treating as zero those singular
   values which are less than rcond times the largest singular value.

   Example Program Results.
   ========================

 LAPACKE_cgelsd (row-major, high-level) Example Program Results

 Minimum norm solution
 ( -0.08,  0.09) (  0.04,  0.16)
 ( -0.17,  0.10) (  0.17, -0.47)
 ( -0.92, -0.01) (  0.71, -0.41)
 ( -0.47, -0.26) (  0.69,  0.02)

 Effective rank =      3

 Singular values
  20.01  18.21   7.88
*/
#include <stdlib.h>
#include <stdio.h>
#include "mkl_lapacke.h"

/* Auxiliary routines prototypes */
extern void print_matrix( char* desc, MKL_INT m, MKL_INT n, MKL_Complex8* a, MKL_INT lda );
extern void print_rmatrix( char* desc, MKL_INT m, MKL_INT n, float* a, MKL_INT lda );

/* Parameters */
#define M 3
#define N 4
#define NRHS 2
#define LDA N
#define LDB NRHS

/* Main program */
int main() {
	/* Locals */
	MKL_INT m = M, n = N, nrhs = NRHS, lda = LDA, ldb = LDB, info, rank;
	/* Negative rcond means using default (machine precision) value */
	float rcond = -1.0;
	/* Local arrays */
	float s[M];
	MKL_Complex8 a[LDA*M] = {
	   { 4.55f, -0.32f}, {-4.36f, -4.76f}, { 3.99f, -6.84f}, { 8.03f, -6.47f},
	   { 8.87f, -3.11f}, { 0.02f,  8.43f}, { 5.43f, -9.30f}, { 2.28f,  8.94f},
	   {-0.74f,  1.16f}, { 3.80f, -6.12f}, {-7.24f,  0.72f}, { 2.21f,  9.52f}
	};
	MKL_Complex8 b[LDB*N] = {
	   {-8.25f,  7.98f}, { 2.91f, -8.81f},
	   {-5.04f,  3.33f}, { 6.19f,  0.19f},
	   { 7.98f, -4.38f}, {-5.96f,  7.18f},
	   { 0.00f,  0.00f}, { 0.00f,  0.00f}
	};
	/* Executable statements */
	printf( "LAPACKE_cgelsd (row-major, high-level) Example Program Results\n" );
	/* Solve the equations A*X = B */
	info = LAPACKE_cgelsd( LAPACK_ROW_MAJOR, m, n, nrhs, a, lda, b, ldb,
			s, rcond, &rank );
	/* Check for convergence */
	if( info > 0 ) {
		printf( "The algorithm computing SVD failed to converge;\n" );
		printf( "the least squares solution could not be computed.\n" );
		exit( 1 );
	}
	/* Print minimum norm solution */
	print_matrix( "Minimum norm solution", n, nrhs, b, ldb );
	/* Print effective rank */
	printf( "\n Effective rank = %6i\n", rank );
	/* Print singular values */
	print_rmatrix( "Singular values", 1, m, s, 1 );
	exit( 0 );
} /* End of LAPACKE_cgelsd Example */

/* Auxiliary routine: printing a matrix */
void print_matrix( char* desc, MKL_INT m, MKL_INT n, MKL_Complex8* a, MKL_INT lda ) {
	MKL_INT i, j;
	printf( "\n %s\n", desc );
	for( i = 0; i < m; i++ ) {
		for( j = 0; j < n; j++ )
			printf( " (%6.2f,%6.2f)", a[i*lda+j].real, a[i*lda+j].imag );
		printf( "\n" );
	}
}

/* Auxiliary routine: printing a real matrix */
void print_rmatrix( char* desc, MKL_INT m, MKL_INT n, float* a, MKL_INT lda ) {
	MKL_INT i, j;
	printf( "\n %s\n", desc );
	for( i = 0; i < m; i++ ) {
		for( j = 0; j < n; j++ ) printf( " %6.2f", a[i*lda+j] );
		printf( "\n" );
	}
}
