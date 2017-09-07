/*******************************************************************************
* Copyright 2005-2017 Intel Corporation All Rights Reserved.
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
!   Content : Intel(R) MKL Extended Eigensolvers C example
!
!*******************************************************************************
!
! Example program for using Intel(R) MKL Extended Eigensolvers (sparse format).
! 
! The following routine are used in the example:
!          SGEMM MKL_SCSRMM  MKL_CCSRADD PARDISO  SFEAST_SRCI.
!
! Consider the matrix A 
!
!                 |  5   2   1   1   0   0   0   0   0   0   0  |
!                 |  2   6   3   1   1   0   0   0   0   0   0  |
!                 |  1   3   6   3   1   1   0   0   0   0   0  |
!                 |  1   1   3   6   3   1   1   0   0   0   0  |
!                 |  0   1   1   3   6   3   1   1   0   0   0  |
!    A    =       |  0   0   1   1   3   6   3   1   1   0   0  |,
!                 |  0   0   0   1   1   3   6   3   1   1   0  |
!                 |  0   0   0   0   1   1   3   6   3   1   1  |
!                 |  0   0   0   0   0   1   1   3   6   3   1  |
!                 |  0   0   0   0   0   0   1   1   3   6   2  |
!                 |  0   0   0   0   0   0   0   1   1   2   5  |
!
!
! stored as sparse matrix  matrix (SINGLE PRECISION version).
! B is a unit matrix:
!
!                 |  1   0   0   0   0   0   0   0   0   0   0  |
!                 |  0   1   0   0   0   0   0   0   0   0   0  |
!                 |  0   0   1   0   0   0   0   0   0   0   0  |
!                 |  0   0   0   1   0   0   0   0   0   0   0  |
!                 |  0   0   0   0   1   0   0   0   0   0   0  |
!    B    =       |  0   0   0   0   0   1   0   0   0   0   0  |.
!                 |  0   0   0   0   0   0   1   0   0   0   0  |
!                 |  0   0   0   0   0   0   0   1   0   0   0  |
!                 |  0   0   0   0   0   0   0   0   1   0   0  |
!                 |  0   0   0   0   0   0   0   0   0   1   0  |
!                 |  0   0   0   0   0   0   0   0   0   0   1  |
!
!  In what follows the symbol ' represents a transpose operation.
!
!  The test performs the following operations :
!
!       1. The code calls  FEASTINIT  to define the default values for the input
!          FEAST parameters.
!
!       2. The  code solves  the generalized eigenvalue problem  Ax=eBx using
!          SFEAST_SRCI.
!
!       3. The code computes the residual R(i) = | E(i) - Eig(i) |  where Eig(i) 
!           are the expected eigenvalues  and E(i) are eigenvalues computed 
!           with the help of SFEAST_SRCI().
!
!       4. The code computes the maximum absolute value of the matrix  Y=(X')*X-I
!          where X is the matrix of eigenvectors computed with the help of 
!          SFEAST_SRCI. SGEMM (BLAS Level 3 Routine) is called  to compute (X')*X.
!
!*******************************************************************************/
#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include "mkl.h"

#define max(a, b) (a) < (b) ? (b): (a)

int main()
{
    //!!!!!!!!!!!!!!! Matrix declaration variables !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    const MKL_INT N = 11;
    float         val[38], valb[11];
    MKL_Complex8  cval[38], cvalb[11], cvalz[38], caux[8*11];
    MKL_INT       rows[12], cols[38], crows[12], ccols[38], rowsb[12], \
                  colsb[11], rowsz[12], colsz[38];
    //!!!!!!!!!!!!!!! Declaration of FEAST variables !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!! E - eigenvalues, X - eigenvectors, res - residual !!!!!!!!!!!!
    MKL_INT       fpm[128];
    float         Emin, Emax;
    float         epsout;
    MKL_INT       loop;
    MKL_INT       ijob;
    MKL_Complex8  Ze;
    float         work[8*11];
    MKL_Complex8  workc[8*11];
    float         Aq[8*8], Sq[8*8];
    MKL_INT       L = 8;
    MKL_INT       M0, M, info;
    float         E[11];
    float         X[121];
    float         res[11];
    //!!!!!!!!!!!!!!! Declaration of PARDISO variables !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!! Internal solver memory pointer pt,                                     !!!
    //!!! 32-bit: int pt[64]; 64-bit: long int pt[64]                            !!!
    //!!! or void *pt[64] should be OK on both architectures                     !!!
    void *pt[64];
    //!!! Pardiso control parameters
    MKL_INT iparm[64];
    MKL_INT phase, maxfct, mnum, mtype, msglvl, error;
    MKL_INT idum = 0; /* Integer dummy. */
    //!!!!!!!!!!!!!!! Declaration of local variables !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!! Eig - array for storing exact eigenvalues, R=|E-Eig|, Y=(X')*X-I !!!!!!!!!
    float         Eig[11];
    float         R[11];
    float         Y[11][11];
    MKL_INT       i,j;
    MKL_INT       ldx = 11, ldy = 11;
    float         trace;
    float         smax, eigabs;
    char          SGEMMC = 'T', SGEMMN = 'N';
    float         one, zero;
    MKL_INT       ione = 1;
    char          transa;
    MKL_INT       job, sort;
    MKL_Complex8  zone;
    char          matdescra[6];
    MKL_INT       colsX, imem, nnzmax;

    printf("\n    FEAST SFEAST_SRCI EXAMPLE PROGRAM\n"); 
    one = (float)1.0;
    zero = (float)0.0;
    zone.real = (float)1.0;
    zone.imag = (float)0.0;

    //!!!!!!!!!!!!!!! Exact eigenvalues in range (3.0, 7.0) !!!!!!!!!!!!!!!!!!!!!!!!
    for ( i=0; i<N; i++ )
        Eig[i] = (float)0.0;

    Eig[0] = (float)3.1715728752538100;
    Eig[1] = (float)4.0000000000000000;
    Eig[2] = (float)4.0000000000000000;
    Eig[3] = (float)4.1292484841890931;
    Eig[4] = (float)4.4066499006731521;
    Eig[5] = (float)6.0000000000000000;

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!!!!   Initialize matrices     !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    for ( i=0; i<N*N; i++ )
    {
        X[i] = zero;
    }

    //!!!!!!!  Initialize upper part of symmetric matrix A   !!!!!!!!!!!!!!!!!!!!!!!
    val[0] = (float)5.0;
    val[1] = (float)2.0;
    val[2] = (float)1.0;
    val[3] = (float)1.0;
    for ( i=1; i<9; i++ )
    {
        val[i*4] = (float)6.0;
        val[i*4+1] = (float)3.0;
        val[i*4+2] = (float)1.0;
        val[i*4+3] = (float)1.0;
    }
    val[35] = (float)6.0;
    val[36] = (float)2.0;
    val[37] = (float)5.0;

    rows[0]  = 1;
    for ( i=1; i<9; i++ )
    {
        rows[i]  = rows[i-1]+4;
    }
    rows[9]  = rows[8]+3;
    rows[10] = rows[9]+2;
    rows[11] = 39;

    for ( i=0; i<9; i++ )
    {
        cols[i*4] = i+1;
        cols[i*4+1] = i+2;
        cols[i*4+2] = i+3;
        cols[i*4+3] = i+4;
    }
    cols[35] = 10;
    cols[36] = 11;
    cols[37] = 11;

    //!!!!!!!!!!!!!!!!!!    Initialize unit matrix B   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    for ( i=0; i<N; i++ )
    {
        valb[i]  = (float)1.0;
        colsb[i] = i+1;
        rowsb[i] = i+1;
    }
    rowsb[N] = N+1;

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!  Initialize upper part of complex symmetric matrix -A !!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    for ( i=0; i<38; i++ )
    {
        cval[i].real = -val[i];
        cval[i].imag = (float)0.0;
    }

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!   Print matrix dimension          !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    printf("Sparse matrix size  %i \n", N);

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!      Testing FEAST sparse format drivers !!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    //!!!!!!!!!!!!!! Search interval [Emin,Emax] including M eigenpairs !!!!!!!!!!!!
    Emin = (float)3.0;
    Emax = (float)7.0;
    M0 = L;
    M = M0;
    printf(" Search interval [ %.7e, %.7e ]  \n", Emin, Emax);
    epsout = (float)0.0;
    loop = 0;
    info = 0;
    //!!!!!!!!!!!!!!           Initialize PARDISO                   !!!!!!!!!!!!!!!!
    mtype = 6; /* Complex symmetric matrix */
    pardisoinit(pt,&mtype,iparm);
    //         Copies all real array valb to complex array cvalb 
    for ( i=0; i<N; i++ )
    {
        cvalb[i].real = valb[i];
        cvalb[i].imag = (float)0.0;
    }
    //
    //         Task 1. Initialize FEAST to define the default values for the input
    //         FEAST parameters.
    //
    ijob = -1;
    info = 0;
    feastinit(fpm);
    fpm[0] = 1; /* Generate runtime messages */
    fpm[1] = 24;
    fpm[6] = 4;
    //
    //         Task 2. The  code solves  the generalized eigenvalue problem  Ax=eBx using
    //         SFEAST_SRCI.
    //
    while ( ijob != 0 )
    {
        sfeast_srci(&ijob,&N,&Ze,work,workc,Aq,Sq,fpm,&epsout,&loop,&Emin,&Emax,&M0,E,X,&M,res,&info);
        if ( info != 0 )
        {
            printf("SFEAST_SRCI info %i \n", info);
            return 1;
        }
        switch ( ijob )
        {
            case -2:
                //!!!!!!!!!!!!!!  New loop                 !!!!!!!!!!!!!!!!!!!!!!!!!
                break; 
            case 0:
                //!!!!!!!!!!!!!!  End                      !!!!!!!!!!!!!!!!!!!!!!!!!
                break; 
            case 10:
                //!!!!!!!!!!!!!!  We can do nothing here   !!!!!!!!!!!!!!!!!!!!!!!!!
                break; 
            case 11:
                //!!!!!!!!!!!!!!  Find matrix ZeB-A        !!!!!!!!!!!!!!!!!!!!!!!!!
                transa = 'N';
                job    = 0;   /* The routine performs addition only     */
                sort   = 0;   /* No reordering for A and B matrices     */
                nnzmax = 38;  /* The length of the cvalsz, colsz arrays */
                error  = 0;   /* Initialize error flag                  */
                mkl_ccsradd(&transa,&job,&sort,&N,&N,cval,cols,rows,&Ze,cvalb,colsb,rowsb,cvalz,colsz,rowsz,&nnzmax,&error);
                if ( error != 0 )
                {
                    printf("MKL_CCSRADD error %d \n", error);
                    return 1;
                }
                //!!!!!!!!!!!!!! Solve (ZeB-A) caux = workc[0:N-1][0:M0-1] !!!!!!!!!!!!!!!!!!!!!
                //!!!!!!!!!!!!!! and put result into workc                 !!!!!!!!!!!!!!!!!!!!!
                phase       = 13;/* Analysis, numerical factorization, solve */
                maxfct      = 1; /* Maximal number of factors in memory      */
                mnum        = 1; /* The number of matrix to solve            */
                msglvl      = 0; /* No print statistical information         */
                iparm[5]    = 1; /* Solution and rhs are input/output        */
                iparm[34]   = 0; /* One-based columns and rows indexing      */
                iparm[28-1] = 1; /* Single precision for Pardiso             */
                pardiso(pt,&maxfct,&mnum,&mtype,&phase,&N,cvalz,rowsz,colsz,&idum,&M0,iparm,&msglvl,workc,caux,&error);
                if ( error != 0 )
                {
                    printf("PARDISO error %d \n", error);
                    return 1;
                }
                break;
            case 30:
                //!!!!!!!!!!!!! Perform multiplication A x[0:N-1][i:j]      !!!!!!!!!!!!!!!!!!!!
                //!!!!!!!!!!!!! and put result into work[0:N-1][i:j]        !!!!!!!!!!!!!!!!!!!!
                //!!!!!!!!!!!!! where i = fpm[23]-1, j = fpm[23]+fpm[24]-2  !!!!!!!!!!!!!!!!!!!!
                transa = 'N'; /* No transpose A */
                matdescra[0] = 'S'; /* Symmetric matrix     */
                matdescra[1] = 'U'; /* Upper part of matrix */
                matdescra[2] = 'N'; /* Non unit             */
                matdescra[3] = 'F'; /* One-based indexing   */
                matdescra[4] = 'x'; /* No value             */
                matdescra[5] = 'x'; /* No value             */
                colsX = fpm[24];
                imem = N*(fpm[23]-1);
                mkl_scsrmm(&transa,&N,&colsX,&N,&one,matdescra,val,cols,rows,rows+1,X+imem,&N,&zero,work+imem,&N);
                break;
            case 40:
                //!!!!!!!!!!!!! Perform multiplication B x[0:N-1][i:j]      !!!!!!!!!!!!!!!!!!!!
                //!!!!!!!!!!!!! and put result into work[0:N-1][i:j]        !!!!!!!!!!!!!!!!!!!!
                //!!!!!!!!!!!!! where i = fpm[23]-1, j = fpm[23]+fpm[24]-2  !!!!!!!!!!!!!!!!!!!!
                transa = 'N'; /* No transpose B */
                matdescra[0] = 'G'; /* general matrix     */
                matdescra[1] = 'x'; /* No value           */
                matdescra[2] = 'x'; /* No value           */
                matdescra[3] = 'F'; /* one-based indexing */
                matdescra[4] = 'x'; /* No value           */
                matdescra[5] = 'x'; /* No value           */
                colsX = fpm[24];
                imem = N*(fpm[23]-1);
                mkl_scsrmm(&transa,&N,&colsX,&N,&one,matdescra,valb,colsb,rowsb,rowsb+1,X+imem,&N,&zero,work+imem,&N);
                break;
            default:
                printf("Wrong ijob %i", ijob); fflush(0);
                return 1;
        }
    }
    //!!!!!!!!!!!!!!! Release memory                              !!!!!!!!!!!!!!!!!!!!
    phase = -1;
    pardiso(pt,&maxfct,&mnum,&mtype,&phase,&N,val,rows,cols,&idum,&M0,iparm,&msglvl,workc,caux,&error);
    if ( error != 0 )
    {
        printf("PARDISO RELEASE MEMORY ERROR %d \n", error);
        return 1;
    }
    //
    //         Task 3. Compute the residual R(i) = | E(i) - Eig(i) |  where Eig(i)
    //         are the expected eigenvalues  and E(i) are eigenvalues computed
    //         with the help of  SFEAST_SRCI().
    //
    printf("  Number of eigenvalues found %d \n", M);
    printf("   Computed    |    Expected  \n");
    printf("   Eigenvalues |    Eigenvalues \n");
    eigabs = (float)0.0;
    for ( i=0; i<M; i++ )
    {
        R[i] = fabs(E[i]-Eig[i]);
        eigabs = max(eigabs, R[i]);
        printf("%.7e %.7e \n", E[i], Eig[i]); 
    }
    printf(" Max value of | computed eigenvalue -expected eigenvalues | %.7e \n", eigabs); 
    //
    //         Task 4.  Compute the maximum absolute value of the matrix
    //         Y=(X')*X-I  where X is the matrix of eigenvectors computed with
    //         the help of SFEAST_SCRCI. 
    //         Call SGEMM (BLAS Level 3 Routine) to compute (X')*X.
    //
    sgemm(&SGEMMC,&SGEMMN,&M,&M,&N,&one,X,&ldx,X,&ldx,&zero,Y,&ldy);

    //          Compute Y=Y-I.
    for ( i=0; i<M; i++ )
        Y[i][i] = Y[i][i]-(float)1.0;
    printf("*************************************************\n");
    printf("************** REPORT ***************************\n");
    printf("*************************************************\n");
    printf("# Search interval [Emin,Emax] %.7e %.7e\n",Emin,Emax);
    printf("# mode found/subspace %d %d \n",M,M0);
    printf("# iterations %d \n",loop);
    trace=(float)0.0;
    for ( i=0; i<M; i++ )
    {
        trace = trace+E[i];
    }
    printf("TRACE %.7e \n", trace);
    printf("Relative error on the Trace %.7e \n",epsout );
    printf("Eigenvalues/Residuals\n");
    for ( i=0; i<M; i++ )
    {
        printf("   %d  %.7e %.7e \n",i,E[i],res[i]);
    }
    smax = (float)0.0;
    for ( i=0; i<M; i++ )
    {
        for ( j=0; j<M; j++ )
        {
            smax = max(smax, fabs(Y[i][j])); 
        }
    }
    printf( "Max of (transposed of X)*X-I %.7e \n", smax);
    return 0;
}
