/*******************************************************************************
* Copyright 2013-2017 Intel Corporation All Rights Reserved.
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
*   Content : Intel(R) MKL SpMV Format Prototype Package C native example
*
********************************************************************************
*/
/*
!
! Consider the matrix A (see 'Sparse Storage Formats for Sparse BLAS Level 2
! and Level 3 in the Intel(R) MKL Reference Manual')
!
!                 |   1       -1      0   -3     0   |
!                 |  -2        5      0    0     0   |
!   A    =        |   0        0      4    6     4   |,
!                 |  -4        0      2    7     0   |
!                 |   0        8      0    0    -5   |
!
!  The matrix A is represented in a zero-based compressed sparse row (CSR) storage
!  scheme with three arrays (see 'Sparse Matrix Storage Schemes' in the
!  Intel(R) MKL Reference Manual) as follows:
!
!         values  =  ( 1 -1 -3 -2  5  4  6  4 -4  2  7  8 -5 )
!         columns =  ( 0  1  3  0  1  2  3  4  0  2  3  1  4 )
!         rowIndex = ( 0        3     5        8       11    13 )
!
!  The test performs the operation
!
!       L^{-1}*x = y, using mkl_sparse_d_trsv
!
!  then checks the result:
!    1. compute:
!       L*y = y1, using mkl_sparse_d_mv
!
!    2. compare x with y1 - x should be equal y1 (x = L*L^{-1}*x)
!
!  Here A is a general sparse matrix
!        L is the lower triangular part of matrix A with unit diagonal
!        x, y and y1 are vectors
!
!*******************************************************************************
*/
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "mkl_spblas.h"

int main() {
    //*******************************************************************************
    //     Declaration and initialization of parameters for sparse representation of
    //     the matrix A in the compressed sparse row format:
    //*******************************************************************************
#define M 5
#define N 5
#define NNZ 13
    //*******************************************************************************
    //    Sparse representation of the matrix A
    //*******************************************************************************
    double csrVal[NNZ]    = { 1.0, -1.0,     -3.0,
                             -2.0,  5.0,
                                         4.0, 6.0, 4.0,
                             -4.0,       2.0, 7.0,
                                    8.0,          -5.0 };
    MKL_INT    csrColInd[NNZ] = { 0,      1,        3,
                              0,      1,
                                           2,   3,   4,
                              0,           2,   3,
                                      1,             4 };
    MKL_INT    csrRowPtr[M+1] = { 0, 3, 5, 8, 11, 13 };
    // Descriptor of main sparse matrix properties
    struct matrix_descr descrA;
    // Structure with sparse matrix stored in CSR format
    sparse_matrix_t       csrA;
    //*******************************************************************************
    //    Declaration of local variables:
    //*******************************************************************************
    double x[N]  = { 1.0, 5.0, 1.0, 4.0, 1.0};
    double y[N]  = { 0.0, 0.0, 0.0, 0.0, 0.0};
    double y1[N]  = { 0.0, 0.0, 0.0, 0.0, 0.0};
    double alpha = 1.0, beta = 0.0, error = 0.0;
    MKL_INT    i;

    printf( "\n EXAMPLE PROGRAM FOR mkl_sparse_d_trsv \n" );
    printf( "---------------------------------------------------\n" );
    printf( "\n" );
    printf( "   INPUT DATA FOR mkl_sparse_d_trsv    \n" );
    printf( "   LOWER TRIANGULAR SPARSE MATRIX \n" );
    printf( "   WITH UNIT DIAGONAL             \n" );
    printf( "   ALPHA = %4.1f  BETA = %4.1f    \n", alpha, beta );
    printf( "   SPARSE_OPERATION_NON_TRANSPOSE \n" );
    printf( "   Input vector                   \n" );
    for ( i = 0; i < N; i++ )
    {
        printf( "%7.1f\n", x[i] );
    };

    // Create handle with matrix stored in CSR format
    mkl_sparse_d_create_csr ( &csrA, SPARSE_INDEX_BASE_ZERO,
                                    N,  // number of rows
                                    M,  // number of cols
                                    csrRowPtr,
                                    csrRowPtr+1,
                                    csrColInd,
                                    csrVal );

    // Create matrix descriptor
    descrA.type = SPARSE_MATRIX_TYPE_TRIANGULAR;
    descrA.mode = SPARSE_FILL_MODE_LOWER;
    descrA.diag = SPARSE_DIAG_UNIT;

    // Compute y = alpha * A^{-1} * x
    mkl_sparse_d_trsv ( SPARSE_OPERATION_NON_TRANSPOSE,
                        alpha,
                        csrA,
                        descrA,
                        x,
                        y );

    printf( "                                   \n" );
    printf( "   OUTPUT DATA FOR mkl_sparse_d_trsv \n" );

    // y should be equal { 1.0, 7.0, 1.0, 6.0, -55.0 }
    for ( i = 0; i < N; i++ )
    {
        printf( "%7.1f\n", y[i] );
    };

    // For validation perform y1 = A * y
    mkl_sparse_d_mv ( SPARSE_OPERATION_NON_TRANSPOSE,
                      alpha,
                      csrA,
                      descrA,
                      y,
                      beta,
                      y1 );

    printf( "                                   \n" );
    printf( "   OUTPUT DATA FOR mkl_sparse_d_mv \n" );

    // y should be equal { 1.0, 7.0, 1.0, 6.0, -55.0 }
    for ( i = 0; i < N; i++ )
    {
        printf( "%7.1f\n", y1[i] );
    };

    // Release matrix handle and deallocate matrix
    mkl_sparse_destroy ( csrA );

    for ( i = 0; i < N; i++ )
    {
        error += x[i]-y1[i];
    };
    if (error > 1e-8)
    {
        printf("\n   VALIDATION FAILED\n");
    } else
    {
        printf("\n   VALIDATION PASSED\n");
    }
    printf( "---------------------------------------------------\n" );
    return 0;
}