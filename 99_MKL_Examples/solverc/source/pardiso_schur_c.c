/*******************************************************************************
* Copyright 2004-2017 Intel Corporation All Rights Reserved.
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
*   Content : Intel(R) MKL PARDISO C example
*
********************************************************************************
*/
/* -------------------------------------------------------------------- */
/* Example program to show the usage of the "PARDISO" routine */
/* for Schur complement solve on symmetric linear systems */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mkl.h"

MKL_INT main (void)
{
    /* Matrix data. */
    MKL_INT n = 8;
    MKL_INT ia[9] = { 1, 5, 8, 10, 12, 15, 17, 18, 19};
    MKL_INT ja[18] = { 1, 3, 6, 7,
        2, 3, 5,
        3, 8,
        4, 7,
        5, 6, 7,
        6, 8,
        7,
        8
    };
    double a[18] = { 7.0, 1.0, 2.0, 7.0,
        -4.0, 8.0, 2.0,
        1.0, 5.0,
        7.0, 9.0,
        5.0, 1.0, 5.0,
        -1.0, 5.0,
        11.0,
        5.0
    };
    int matrix_order=LAPACK_ROW_MAJOR;
    char uplo = 'U';
    MKL_INT mtype = -2;   /* Real symmetric matrix */
    /* RHS and solution vectors. */
    double b[8], x[8];
    MKL_INT nrhs = 1;     /* Number of right hand sides. */
    /* Internal solver memory pointer pt, */
    /* 32-bit: int pt[64]; 64-bit: long int pt[64] */
    /* or void *pt[64] should be OK on both architectures */
    void *pt[64];
    /* Pardiso control parameters. */
    MKL_INT iparm[64];
    MKL_INT maxfct, mnum, phase, error, msglvl, info;
    /* Auxiliary variables. */
    MKL_INT i, j;
    double ddum;          /* Double dummy */
    MKL_INT idum;         /* Integer dummy. */
    /* Schur data */
    double schur[4] = {0.0, 0.0,
                       0.0, 0.0 };
    MKL_INT perm[8] = {0, 0, 0, 0, 0, 0, 1, 1};
    MKL_INT ipiv[2];
    MKL_INT n_schur = 2; /* Schur complement solution size */
/* -------------------------------------------------------------------- */
/* .. Setup Pardiso control parameters. */
/* -------------------------------------------------------------------- */
    for ( i = 0; i < 64; i++ )
    {
        iparm[i] = 0;
    }
    iparm[1-1] = 1;         /* No solver default */
    iparm[2-1] = 2;         /* Fill-in reordering from METIS */
    iparm[10-1] = 8;        /* Perturb the pivot elements with 1E-13 */
    iparm[11-1] = 0;        /* Use nonsymmetric permutation and scaling MPS */
    iparm[13-1] = 0;        /* Maximum weighted matching algorithm is switched-off (default for symmetric). Try iparm[12] = 1 in case of inappropriate accuracy */
    iparm[14-1] = 0;        /* Output: Number of perturbed pivots */
    iparm[18-1] = -1;       /* Output: Number of nonzeros in the factor LU */
    iparm[19-1] = -1;       /* Output: Mflops for LU factorization */
    iparm[36-1] = 1;        /* Use Schur complement */

    maxfct = 1;           /* Maximum number of numerical factorizations. */
    mnum = 1;             /* Which factorization to use. */
    msglvl = 1;           /* Print statistical information in file */
    error = 0;            /* Initialize error flag */
/* -------------------------------------------------------------------- */
/* .. Initialize the internal solver memory pointer. This is only */
/* necessary for the FIRST call of the PARDISO solver. */
/* -------------------------------------------------------------------- */
    for ( i = 0; i < 64; i++ )
    {
        pt[i] = 0;
    }
/* -------------------------------------------------------------------- */
/* .. Reordering and Symbolic Factorization. This step also allocates   */
/* all memory that is necessary for the factorization.                  */
/* -------------------------------------------------------------------- */
    phase = 11;
    pardiso (pt, &maxfct, &mnum, &mtype, &phase,
             &n, a, ia, ja, perm, &nrhs, iparm, &msglvl, &ddum, &ddum, &error);
    if ( error != 0 )
    {
        printf ("\nERROR during symbolic factorization: %d", error);
        exit (1);
    }
    printf ("\nReordering completed ... ");
/* -------------------------------------------------------------------- */
/* .. Numerical factorization. */
/* -------------------------------------------------------------------- */
    phase = 22;
    pardiso(pt, &maxfct, &mnum, &mtype, &phase,
            &n, a, ia, ja, perm, &nrhs,
            iparm, &msglvl, &ddum, &schur, &error);
    if ( error != 0 )
    {
        printf ("\nERROR during numerical factorization: %d", error);
        exit (2);
    }
    printf ("\nFactorization completed ... ");
    printf("\nSchur matrix: \n"); fflush(0);
    for(i=0; i<n_schur; i++)
    {
        for(j=0; j<n_schur; j++)
        {
            printf("%e ",schur[i*n_schur+j]); fflush(0);
        }
        printf("\n"); fflush(0);
    }
/* -------------------------------------------------------------------- */
/* .. Reduce solving phase. */
/* -------------------------------------------------------------------- */
    phase = 331;
    /* Set right hand side to one. */
    for ( i = 0; i < n; i++ )
    {
        b[i] = 1.0;
    }
    pardiso(pt, &maxfct, &mnum, &mtype, &phase,
            &n, a, ia, ja, perm, &nrhs,
            iparm, &msglvl, b, x, &error);
    if ( error != 0 )
    {
        printf ("\nERROR during solution phase 331: %d", error);
        exit (331);
    }
    for ( i = 0; i < n; i++ )
    {
        b[i] = x[i];
    }
    printf ("\nSolve phase 331 completed ... ");
/* -------------------------------------------------------------------- */
/* .. Solving Schur complement. */
/* -------------------------------------------------------------------- */
    for(i = 0; i < n_schur; i++) ipiv[i] = 0;
    info = LAPACKE_dsytrf(matrix_order,uplo,n_schur,&schur,n_schur,ipiv);
    if(info != 0)
    {
        printf("info after LAPACKE_dsytrf = %d \n", info);
        return 1;
    }
    info = LAPACKE_dsytrs(matrix_order,uplo,n_schur,nrhs,&schur,n_schur,&ipiv,&b[n-n_schur],nrhs);
    if(info != 0)
    {
        printf("info after LAPACKE_dsytrs = %d \n", info);
        return 1;
    }
/* -------------------------------------------------------------------- */
/* .. Expansion solving phase. */
/* -------------------------------------------------------------------- */
    phase = 333;
    /* Set right hand side to x one. */
    pardiso(pt, &maxfct, &mnum, &mtype, &phase,
            &n, a, ia, ja, perm, &nrhs, 
            iparm, &msglvl, b, x, &error);
    if ( error != 0 )
    {
        printf ("\nERROR during solution: %d", error);
        exit (333);
    }
    printf ("\nSolve phase 333 completed ... ");
    printf ("\nSolve completed ... ");
    printf ("\nThe solution of the system is: ");
    for ( i = 0; i < n; i++ )
    {
        printf ("\n x [%d] = % f", i, x[i]);
    }
    printf ("\n");
/* -------------------------------------------------------------------- */
/* .. Termination and release of memory. */
/* -------------------------------------------------------------------- */
    phase = -1;           /* Release internal memory. */
    pardiso(pt, &maxfct, &mnum, &mtype, &phase,
             &n, &ddum, ia, ja, &idum, &nrhs,
             iparm, &msglvl, &ddum, &ddum, &error);
    return 0;
}
