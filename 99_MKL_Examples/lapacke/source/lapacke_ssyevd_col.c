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
   LAPACKE_ssyevd Example.
   =======================

   Program computes all eigenvalues and eigenvectors of a real symmetric
   matrix A using divide and conquer algorithm, where A is:

     6.39   0.13  -8.23   5.71  -3.18
     0.13   8.37  -4.46  -6.10   7.21
    -8.23  -4.46  -9.58  -9.25  -7.42
     5.71  -6.10  -9.25   3.72   8.54
    -3.18   7.21  -7.42   8.54   2.51

   Description.
   ============

   The routine computes all eigenvalues and, optionally, eigenvectors of an
   n-by-n real symmetric matrix A. The eigenvector v(j) of A satisfies

   A*v(j) = lambda(j)*v(j)

   where lambda(j) is its eigenvalue. The computed eigenvectors are
   orthonormal.
   If the eigenvectors are requested, then this routine uses a divide and
   conquer algorithm to compute eigenvalues and eigenvectors.

   Example Program Results.
   ========================

 LAPACKE_ssyevd (column-major, high-level) Example Program Results

 Eigenvalues
 -17.44 -11.96   6.72  14.25  19.84

 Eigenvectors (stored columnwise)
  -0.26   0.31  -0.74   0.33   0.42
  -0.17  -0.39  -0.38  -0.80   0.16
  -0.89   0.04   0.09   0.03  -0.45
  -0.29  -0.59   0.34   0.31   0.60
  -0.19   0.63   0.44  -0.38   0.48
*/
#include <stdlib.h>
#include <stdio.h>
#include "mkl_lapacke.h"

/* Auxiliary routines prototypes */
extern void print_matrix( char* desc, MKL_INT m, MKL_INT n, float* a, MKL_INT lda );

/* Parameters */
#define N 5
#define LDA N

/* Main program */
int main() {
	/* Locals */
	MKL_INT n = N, lda = LDA, info;
	/* Local arrays */
	float w[N];
	float a[LDA*N] = {
	    6.39f,  0.00f,  0.00f,  0.00f,  0.00f,
	    0.13f,  8.37f,  0.00f,  0.00f,  0.00f,
	   -8.23f, -4.46f, -9.58f,  0.00f,  0.00f,
	    5.71f, -6.10f, -9.25f,  3.72f,  0.00f,
	   -3.18f,  7.21f, -7.42f,  8.54f,  2.51f
	};
	/* Executable statements */
	printf( "LAPACKE_ssyevd (column-major, high-level) Example Program Results\n" );
	/* Solve eigenproblem */
	info = LAPACKE_ssyevd( LAPACK_COL_MAJOR, 'V', 'U', n, a, lda, w );
	/* Check for convergence */
	if( info > 0 ) {
		printf( "The algorithm failed to compute eigenvalues.\n" );
		exit( 1 );
	}
	/* Print eigenvalues */
	print_matrix( "Eigenvalues", 1, n, w, 1 );
	/* Print eigenvectors */
	print_matrix( "Eigenvectors (stored columnwise)", n, n, a, lda );
	exit( 0 );
} /* End of LAPACKE_ssyevd Example */

/* Auxiliary routine: printing a matrix */
void print_matrix( char* desc, MKL_INT m, MKL_INT n, float* a, MKL_INT lda ) {
	MKL_INT i, j;
	printf( "\n %s\n", desc );
	for( i = 0; i < m; i++ ) {
		for( j = 0; j < n; j++ ) printf( " %6.2f", a[i+j*lda] );
		printf( "\n" );
	}
}