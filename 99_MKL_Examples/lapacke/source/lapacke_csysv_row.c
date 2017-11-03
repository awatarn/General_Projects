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
   LAPACKE_csysv Example.
   ======================
 
   The program computes the solution to the system of linear equations
   with a complex symmetric matrix A and multiple right-hand sides B,
   where A is the coefficient matrix:

   (  9.99, -4.73) ( -5.68, -0.80) ( -8.94,  1.32) ( -9.42,  2.05)
   ( -5.68, -0.80) ( -8.01,  4.61) (  1.64, -6.29) (  6.79, -2.17)
   ( -8.94,  1.32) (  1.64, -6.29) (  9.04,  3.96) ( -4.51, -7.54)
   ( -9.42,  2.05) (  6.79, -2.17) ( -4.51, -7.54) (  0.40,  4.06)

   and B is the right-hand side matrix:
 
   (  5.71, -1.20) (  2.84, -0.18)
   ( -7.70,  6.47) ( -8.29, -1.72)
   (  3.77, -7.40) ( -4.28, -8.25)
   ( -3.78,  0.33) ( -2.70, -0.39)
 
   Description.
   ============
 
   The routine solves for X the complex system of linear equations A*X = B,
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
 
 LAPACKE_csysv (row-major, high-level) Example Program Results

 Solution
 (  0.13,  0.13) (  0.63,  0.34)
 (  0.32, -0.07) (  0.61,  0.21)
 ( -0.26, -0.44) ( -0.01, -0.10)
 ( -0.40,  0.51) (  0.21,  0.02)

 Details of factorization
 (-16.42,  1.69) ( -0.53,  0.35) (  0.36,  0.41) ( -0.78,  0.49)
 (  0.00,  0.00) (  3.69,  0.64) (-16.58, -1.61) ( -0.10, -0.65)
 (  0.00,  0.00) (  0.00,  0.00) (  1.02, -3.74) ( -0.73, -0.52)
 (  0.00,  0.00) (  0.00,  0.00) (  0.00,  0.00) (  9.04,  3.96)

 Pivot indices
      1     -1     -1      3
*/
#include <stdlib.h>
#include <stdio.h>
#include "mkl_lapacke.h"

/* Auxiliary routines prototypes */
extern void print_matrix( char* desc, MKL_INT m, MKL_INT n, MKL_Complex8* a, MKL_INT lda );
extern void print_int_vector( char* desc, MKL_INT n, MKL_INT* a );

/* Parameters */
#define N 4
#define NRHS 2
#define LDA N
#define LDB NRHS

/* Main program */
int main() {
	/* Locals */
	MKL_INT n = N, nrhs = NRHS, lda = LDA, ldb = LDB, info;
	/* Local arrays */
	MKL_INT ipiv[N];
	MKL_Complex8 a[LDA*N] = {
	   { 9.99f, -4.73f}, {-5.68f, -0.80f}, {-8.94f,  1.32f}, {-9.42f,  2.05f},
	   { 0.00f,  0.00f}, {-8.01f,  4.61f}, { 1.64f, -6.29f}, { 6.79f, -2.17f},
	   { 0.00f,  0.00f}, { 0.00f,  0.00f}, { 9.04f,  3.96f}, {-4.51f, -7.54f},
	   { 0.00f,  0.00f}, { 0.00f,  0.00f}, { 0.00f,  0.00f}, { 0.40f,  4.06f}
	};
	MKL_Complex8 b[LDB*N] = {
	   { 5.71f, -1.20f}, { 2.84f, -0.18f},
	   {-7.70f,  6.47f}, {-8.29f, -1.72f},
	   { 3.77f, -7.40f}, {-4.28f, -8.25f},
	   {-3.78f,  0.33f}, {-2.70f, -0.39f}
	};
	/* Executable statements */
	printf( "LAPACKE_csysv (row-major, high-level) Example Program Results\n" );
	/* Solve the equations A*X = B */
	info = LAPACKE_csysv( LAPACK_ROW_MAJOR, 'U', n, nrhs, a, lda, ipiv,
			b, ldb );
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
	exit( 0 );
} /* End of LAPACKE_csysv Example */

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

/* Auxiliary routine: printing a vector of integers */
void print_int_vector( char* desc, MKL_INT n, MKL_INT* a ) {
	MKL_INT j;
	printf( "\n %s\n", desc );
	for( j = 0; j < n; j++ ) printf( " %6i", a[j] );
	printf( "\n" );
}