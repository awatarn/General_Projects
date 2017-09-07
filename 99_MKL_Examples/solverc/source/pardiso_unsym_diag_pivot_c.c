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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mkl_service.h"

#include "mkl_pardiso.h"
#include "mkl_types.h"

MKL_INT main (void)
{
    /* Matrix data. */
    MKL_INT n = 5;
    MKL_INT ia[6] = { 1, 4, 6, 9, 12, 14};
    MKL_INT ja[13] = 
    { 1, 2,    4,
      1, 2,
            3, 4, 5,
      1,    3, 4,
         2,       5
    };
    double a1[13] = 
    { 21.0,-1.0,     -3.0,
     -2.0, 35.0,
                44.0, 6.0, 4.0,
     -4.0,      2.0, 57.0,
           8.0,          -65.0
    };

	double a2[13] = 
    { -21.0,-1.0,     -3.0,
     -2.0, -35.0,
                -44.0, 6.0, 4.0,
     -4.0,      2.0, -57.0,
           8.0,           65.0
    };


    MKL_INT mtype = 11;       /* Real symmetric matrix */
    /* RHS and solution vectors. */
    double b[8], x[8];
    MKL_INT nrhs = 1;     /* Number of right hand sides. */
    double *df = NULL;
    double *da = NULL;
    MKL_INT er_d=0;
    /* Internal solver memory pointer pt, */
    /* 32-bit: int pt[64]; 64-bit: long int pt[64] */
    /* or void *pt[64] should be OK on both architectures */
    void *pt[64];
    /* Pardiso control parameters. */
    MKL_INT iparm[64];
    MKL_INT maxfct, mnum, phase, error, msglvl;
    /* Auxiliary variables. */
    MKL_INT i;
    double ddum;          /* Double dummy */
    MKL_INT idum;         /* Integer dummy. */
/* -------------------------------------------------------------------- */
/* .. Setup Pardiso control parameters. */
/* -------------------------------------------------------------------- */
    for ( i = 0; i < 64; i++ )
    {
        iparm[i] = 0;
    }
    iparm[0] = 1;         /* No solver default */
    iparm[1] = 2;         /* Fill-in reordering from METIS */
    iparm[3] = 0;         /* No iterative-direct algorithm */
    iparm[4] = 0;         /* No user fill-in reducing permutation */
    iparm[5] = 0;         /* Write solution into x */
    iparm[6] = 0;         /* Not in use */
    iparm[7] = 2;         /* Max numbers of iterative refinement steps */
    iparm[8] = 0;         /* Not in use */
    iparm[9] = 13;        /* Perturb the pivot elements with 1E-13 */
    iparm[10] = 1;        /* Use nonsymmetric permutation and scaling MPS */
    iparm[11] = 0;        /* Not in use */
    iparm[12] = 0;        /* Maximum weighted matching algorithm is switched-off (default for symmetric). Try iparm[12] = 1 in case of inappropriate accuracy */
    iparm[13] = 0;        /* Output: Number of perturbed pivots */
    iparm[14] = 0;        /* Not in use */
    iparm[15] = 0;        /* Not in use */
    iparm[16] = 0;        /* Not in use */
    iparm[17] = -1;       /* Output: Number of nonzeros in the factor LU */
    iparm[18] = -1;       /* Output: Mflops for LU factorization */
    iparm[19] = 0;        /* Output: Numbers of CG Iterations */
    iparm[55] = 1;        /* Pivoting control */
    maxfct = 2;           /* Maximum number of numerical factorizations. */
    mnum = 1;         /* Which factorization to use. */
    msglvl = 0;           /* Print statistical information in file */
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
/* .. Reordering and Symbolic Factorization. This step also allocates */
/* all memory that is necessary for the factorization. */
/* -------------------------------------------------------------------- */
    phase = 11;
    PARDISO (pt, &maxfct, &mnum, &mtype, &phase,
             &n, a1, ia, ja, &idum, &nrhs, iparm, &msglvl, &ddum, &ddum, &error);
    if ( error != 0 )
    {
        printf ("\nERROR during symbolic factorization: %d", error);
        exit (1);
    }
    printf ("\nReordering completed ... ");
    printf ("\nNumber of nonzeros in factors = %d", iparm[17]);
    printf ("\nNumber of factorization MFLOPS = %d", iparm[18]);fflush(0);
/* -------------------------------------------------------------------- */
/* .. Numerical factorization. */
/* -------------------------------------------------------------------- */



    phase = 22;
    PARDISO (pt, &maxfct, &mnum, &mtype, &phase,
             &n, a1, ia, ja, &idum, &nrhs, iparm, &msglvl, &ddum, &ddum, &error);
    if ( error != 0 )
    {
        printf ("\nERROR during numerical factorization: %d", error);
        exit (2);
    }
    printf ("\nFirst Factorization completed ... \n");fflush(0);

    phase = 22;
    mnum=2;

    PARDISO (pt, &maxfct, &mnum, &mtype, &phase,
             &n, a2, ia, ja, &idum, &nrhs, iparm, &msglvl, &ddum, &ddum, &error);
    if ( error != 0 )
    {
        printf ("\nERROR during numerical factorization: %d", error);
        exit (2);
    }
    printf ("\nSecond Factorization completed ... \n");fflush(0);

/* -------------------------------------------------------------------- */
/* .. Back substitution and iterative refinement. */
/* -------------------------------------------------------------------- */
    df = (double*) mkl_malloc(n * sizeof( double ), 64);
    if ( NULL == df )
        return 1;
    da = (double*) mkl_malloc(n * sizeof( double ), 64);
    if ( NULL == da )
    {
        mkl_free( df );
        return 2;
    }

    mnum=1;

    pardiso_getdiag(pt, df, da, &mnum, &er_d);
    printf ("\nFirst\n");
    for ( i = 0; i < n; i++ )
    {
        printf ("d_fact[%d]= %lf d_a=%lf \n", i, df[i], da[i]);
    }

    mnum=2;
    pardiso_getdiag(pt, df, da, &mnum, &er_d);
    printf ("\nSecond\n");
    for ( i = 0; i < n; i++ )
    {
        printf ("d_fact[%d]= %lf d_a=%lf \n", i, df[i], da[i]);
    }

/* -------------------------------------------------------------------- */
/* .. Termination and release of memory. */
/* -------------------------------------------------------------------- */
    phase = -1;           /* Release internal memory. */
    PARDISO (pt, &maxfct, &mnum, &mtype, &phase,
             &n, &ddum, ia, ja, &idum, &nrhs,
             iparm, &msglvl, &ddum, &ddum, &error);
    mkl_free( df );
    mkl_free( da );
    return 0;
}

/* -------------------------------------------------------------------- */
/* .. mkl_pardiso_pivot: This function replace appropriate function     */
/* ..                    in pardiso solver                              */
/* -------------------------------------------------------------------- */

int mkl_pardiso_pivot( const double*aii, double*bii, const double*eps ) 
{
    if ( (*bii > *eps) || ( *bii < -*eps ) )
        return 0;
    if ( *bii > 0 )
        *bii = *eps;
    else
        *bii = -*eps;
    return 1;
}
