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
   LAPACKE_ssyevx Example.
   =======================

   Program computes the smallest eigenvalues and the corresponding
   eigenvectors of a real symmetric matrix A:

     6.29  -0.39   0.61   1.18  -0.08
    -0.39   7.19   0.81   1.19  -0.08
     0.61   0.81   5.48  -3.13   0.22
     1.18   1.19  -3.13   3.79  -0.26
    -0.08  -0.08   0.22  -0.26   0.83

   Description.
   ============

   The routine computes selected eigenvalues and, optionally, eigenvectors of
   an n-by-n real symmetric matrix A. The eigenvector v(j) of A satisfies

   A*v(j) = lambda(j)*v(j)

   where lambda(j) is its eigenvalue. The computed eigenvectors are
   orthonormal.
   Eigenvalues and eigenvectors can be selected by specifying either a range
   of values or a range of indices for the desired eigenvalues.

   Example Program Results.
   ========================

 LAPACKE_ssyevx (row-major, high-level) Example Program Results

 The total number of eigenvalues found: 3

 Selected eigenvalues
   0.71   0.82   6.58

 Selected eigenvectors (stored columnwise)
   0.22   0.09  -0.95
   0.21   0.08  -0.04
  -0.52  -0.22  -0.29
  -0.73  -0.21  -0.09
  -0.32   0.94   0.01
*/
#include <stdlib.h>
#include <stdio.h>
#include "mkl_lapacke.h"

/* Auxiliary routines prototypes */
extern void print_matrix( char* desc, MKL_INT m, MKL_INT n, float* a, MKL_INT lda );

/* Parameters */
#define N 5
#define NSELECT 3
#define LDA N
#define LDZ NSELECT

/* Main program */
int main() {
	/* Locals */
	MKL_INT n = N, il, iu, m, lda = LDA, ldz = LDZ, info;
	float abstol, vl = 0.0f, vu = 0.0f;
	/* Local arrays */
	MKL_INT ifail[N];
	float w[N], z[LDZ*N];
	float a[LDA*N] = {
	    6.29f, -0.39f,  0.61f,  1.18f, -0.08f,
	    0.00f,  7.19f,  0.81f,  1.19f, -0.08f,
	    0.00f,  0.00f,  5.48f, -3.13f, 0.22f,
	    0.00f,  0.00f,  0.00f,  3.79f, -0.26f,
	    0.00f,  0.00f,  0.00f,  0.00f, 0.83f
	};
	/* Executable statements */
	printf( "LAPACKE_ssyevx (row-major, high-level) Example Program Results\n" );
	/* Negative abstol means using the default value */
	abstol = -1.0;
	/* Set il, iu to compute NSELECT smallest eigenvalues */
	il = 1;
	iu = NSELECT;
	/* Solve eigenproblem */
	info = LAPACKE_ssyevx( LAPACK_ROW_MAJOR, 'V', 'I', 'U', n, a, lda,
			vl, vu, il, iu, abstol, &m, w, z, ldz, ifail );
	/* Check for convergence */
	if( info > 0 ) {
		printf( "The algorithm failed to compute eigenvalues.\n" );
		exit( 1 );
	}
	/* Print the number of eigenvalues found */
	printf( "\n The total number of eigenvalues found:%2i\n", m );
	/* Print eigenvalues */
	print_matrix( "Selected eigenvalues", 1, m, w, 1 );
	/* Print eigenvectors */
	print_matrix( "Selected eigenvectors (stored columnwise)", n, m, z, ldz );
	exit( 0 );
} /* End of LAPACKE_ssyevx Example */

/* Auxiliary routine: printing a matrix */
void print_matrix( char* desc, MKL_INT m, MKL_INT n, float* a, MKL_INT lda ) {
	MKL_INT i, j;
	printf( "\n %s\n", desc );
	for( i = 0; i < m; i++ ) {
		for( j = 0; j < n; j++ ) printf( " %6.2f", a[i*lda+j] );
		printf( "\n" );
	}
}
