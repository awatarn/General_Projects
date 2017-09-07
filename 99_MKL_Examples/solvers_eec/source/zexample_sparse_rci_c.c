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
! Example program demonstrated the use RCI functionality of
! Intel(R) MKL Extended Eigensolvers (sparse format).
!
! The following routines are used in the example:
!          ZGEMM MKL_ZCSRMM MKL_ZCSRADD ZFEAST_HRCI PARDISO.
!
! Consider the matrix A 
!
!                 |  10   1+2i  0    0    0    0    0    0    0    0    |
!                 |  1-2i  9   2+3i  0    0    0    0    0    0    0    |
!                 |  0    2-3i  8   3+4i  0    0    0    0    0    0    |
!                 |  0    0    3-4i  7   4+5i  0    0    0    0    0    |
!                 |  0    0    0    4-5i  6   5+6i  0    0    0    0    |
!    A    =       |  0    0    0    0    5-6i  5   6+7i  0    0    0    |,
!                 |  0    0    0    0    0    6-7i  4   7+8i  0    0    |
!                 |  0    0    0    0    0    0    7-8i  3   8+9i  0    |
!                 |  0    0    0    0    0    0    0    8-9i  2   9+10i |
!                 |  0    0    0    0    0    0    0    0    9-10i 1    |
!
! stored as sparse matrix (DOUBLE COMPLEX PRECISION version).
! B is a unit matrix:
!
!                 |  1   0   0   0   0   0   0   0   0   0  |
!                 |  0   1   0   0   0   0   0   0   0   0  |
!                 |  0   0   1   0   0   0   0   0   0   0  |
!                 |  0   0   0   1   0   0   0   0   0   0  |
!                 |  0   0   0   0   1   0   0   0   0   0  |
!    B    =       |  0   0   0   0   0   1   0   0   0   0  |.
!                 |  0   0   0   0   0   0   1   0   0   0  |
!                 |  0   0   0   0   0   0   0   1   0   0  |
!                 |  0   0   0   0   0   0   0   0   1   0  | 
!                 |  0   0   0   0   0   0   0   0   0   1  |
!
!  In what follows the symbol ' represents a transpose conjugate operation.
!
!  The test performs the following operations :
!
!       1. The code calls  FEASTINIT  to define the default values for the input
!          FEAST parameters.
!
!       2. The  code solves  the generalized eigenvalue problem  Ax=eBx using
!          ZFEAST_HRCI.
!
!       3. The code computes the residual R(i) = | E(i) - Eig(i) |  where Eig(i)
!           are the expected eigenvalues  and E(i) are eigenvalues computed 
!           with the help of ZFEAST_HRCI().
!
!       4. The code computes the maximum absolute value of the matrix  Y=(X')*X-I
!          where X is the matrix of eigenvectors computed with the help of 
!          ZFEAST_HRCI. ZGEMM (BLAS Level 3 Routine) is called  to compute (X')*X.
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
    const MKL_INT N = 10;
    MKL_Complex16 val[19], nval[19], valb[10], valz[19], caux[8*10];
    MKL_INT       rows[11], cols[19], crows[11], ccols[19], rowsb[11], \
                  colsb[10], rowsz[11], colsz[19];
    //!!!!!!!!!!!!!!! Declaration of FEAST variables !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!! E - eigenvalues, X - eigenvectors, res - residual !!!!!!!!!!!!
    MKL_INT       fpm[128];
    double        Emin=(double)2.0, Emax=(double)12.0;
    double        epsout;
    MKL_INT       loop;
    MKL_INT       ijob;
    MKL_Complex16 Ze;
    MKL_Complex16 workc[8*10], work[8*10];
    MKL_Complex16 Aq[8*8], Sq[8*8];
    MKL_INT       L = 8;
    MKL_INT       M0, M, info;
    double        E[10];
    MKL_Complex16 X[100];
    double        res[10];
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
    double        Eig[10];
    double        R[10];
    MKL_Complex16 Y[10][10];
    MKL_INT       i,j;
    MKL_INT       ldx = 10, ldy = 10;
    double        trace;
    double        smax, eigabs, t1;
    char          CGEMMC = 'C', CGEMMN = 'N';
    MKL_Complex16 one, zero;
    MKL_INT       ione = 1;
    char          transa;
    MKL_INT       job, sort;
    char          matdescra[6];
    MKL_INT       colsX, imem, nnzmax;

    printf("\n    FEAST ZFEAST_HRCI EXAMPLE PROGRAM\n"); 
    one.real  = (double)1.0;
    one.imag  = (double)0.0;
    zero.real = (double)0.0;
    zero.imag = (double)0.0;

    //!!!!!!!!!!!!!!! Exact eigenvalues in range (2.0, 12.0) !!!!!!!!!!!!!!!!!!!!!!
    for ( i=0; i<N; i++ )
        Eig[i]=(double)0.0;

    Eig[0]=(double)2.231050249405326;
    Eig[1]=(double)6.058514949896253;
    Eig[2]=(double)9.109748032279734;
    Eig[3]=(double)11.70314827299435;

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!!     Initialize matrices     !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    for ( i=0; i<N*N; i++ )
    {
        X[i] = zero;
    }

    //!!!!!!!  Initialize upper part of Hermitian matrix A   !!!!!!!!!!!!!!!!!!!!!!!
    for ( i=0; i<9; i++ )
    {
        val[i*2].real = (double)(N-i);
        val[i*2].imag = (double)0.0;
        val[1+i*2].real = (double)(i+1);
        val[1+i*2].imag = (double)(i+2); 
    }
    val[18].real= (double)1.0;
    val[18].imag= (double)0.0;  

    for ( i=0; i<9; i++ )
    {
        cols[i*2]=i+1;
        cols[1+i*2]=i+2;
        rows[i]=2*i+1;
    }
    cols[18]=10;
    rows[9]=19;
    rows[10]=20;

    //!!!!!!!!!!!!!!!!!!    Initialize unit matrix B   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    for ( i=0; i<N; i++ )
    {
        valb[i].real= (double)1.0;
        valb[i].imag= (double)0.0;
        colsb[i] = i+1;
        rowsb[i] = i+1;
    }
    rowsb[N] = N+1;

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!  Initialize Hermitian upper part of matrix -A  !!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    for ( i=0; i<19; i++ )
    {
        nval[i].real= -val[i].real;
        nval[i].imag= -val[i].imag;
    }

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!   Print matrix dimension          !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    printf("Sparse matrix size  %i \n", N);

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!      Testing FEAST sparse format drivers !!!!!!!!!!!!!!!!!!!!!!!!
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    //!!!!!!!!!!!!!! Search interval [Emin,Emax] including M eigenpairs !!!!!!!!!!!!
    M0 = L;
    M = M0;
    printf(" Search interval [ %.15e, %.15e ]  \n", Emin, Emax);
    epsout = (double)0.0;
    loop = 0;
    info = 0;
    //!!!!!!!!!!!!     Initialize PARDISO                   !!!!!!!!!!!!!!!!!!!!!!!!
    mtype = -4; /* Hermitian and nondefinite matrix */
    pardisoinit(pt,&mtype,iparm);
    phase     = 11;/* Analysis                            */
    maxfct    = 1; /* Maximal number of factors in memory */
    mnum      = 1; /* The number of matrix to solve       */
    msglvl    = 0; /* No print statistical information    */
    iparm[5]  = 1; /* Solution and rhs are input/output   */
    iparm[34] = 0; /* One-based columns and rows indexing */
    //
    //         Task 1. Initialize FEAST to define the default values for the input 
    //         FEAST parameters.
    //
    ijob = -1;
    info = 0;
    feastinit(fpm);
    fpm[0] = 1; /* Generate runtime messages */
    fpm[5] = 1; /* Second stopping criteria  */
    //
    //         Task 2. The  code solves  the generalized eigenvalue problem  Ax=eBx using
    //     ZFEAST_HRCI.
    //
    while ( ijob != 0 )
    {
        zfeast_hrci(&ijob,&N,&Ze,work,workc,Aq,Sq,fpm,&epsout,&loop,&Emin,&Emax,&M0,E,X,&M,res,&info);
        if ( info != 0 )
        {
            printf("ZFEAST_HRCI info %i \n", info);
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
                //!!!!!!!!!!!!!!  Factorize matrix ZeB-A   !!!!!!!!!!!!!!!!!!!!!!!!!
                //         Find upper part of matrix ZeB-A
                transa = 'N';
                job = 0;     /* The routine performs addition only    */
                sort = 0;    /* No reordering for A and B matrices    */
                nnzmax = 19; /* The length of the valsz, colsz arrays */
                error = 0;   /* Initialize error flag                 */
                mkl_zcsradd(&transa,&job,&sort,&N,&N,nval,cols,rows,&Ze,valb,colsb,rowsb,valz,colsz,rowsz,&nnzmax,&error);
                if ( error != 0 )
                {
                    printf("MKL_ZCSRADD error %d \n", error);
                    return 1;
                }

                phase = 12; /* Reordering and numerical factorization */
                pardiso(pt,&maxfct,&mnum,&mtype,&phase,&N,valz,rowsz,colsz,&idum,&M0,iparm,&msglvl,workc,caux,&error);
                if ( error != 0 )
                {
                    printf("PARDISO error %d \n", error);
                    return 1;
                }
                break; 
            case 11:
                //!!!!!!! Solve (ZeB-A) caux = workc[0:N-1][0:M0-1] and put result into workc !!
                phase  = 33; /* Solve */
                pardiso(pt,&maxfct,&mnum,&mtype,&phase,&N,valz,rowsz,colsz,&idum,&M0,iparm,&msglvl,workc,caux,&error);
                if ( error != 0 )
                {
                    printf("PARDISO error %d \n", error);
                    return 1;
                }
                break;
            case 20:
                //!!!!!!!!!!!!! Factorize matrix (ZeB-A)^H !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                //         We can do nothing here;
                break; 
            case 21:
                //!!!!! Solve (ZeB-A)^H caux = workc[0:N-1][0:M0-1] and put result into workc !!
                phase  = 33;   /* Solve */
                iparm[11] = 1; /* Transpose conjugate solve */
                pardiso(pt,&maxfct,&mnum,&mtype,&phase,&N,valz,rowsz,colsz,&idum,&M0,iparm,&msglvl,workc,caux,&error);
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
                matdescra[0] = 'H'; /* Hermitian matrix */ 
                matdescra[1] = 'U'; /* Upper part of matrix */
                matdescra[2] = 'N'; /* Non unit */
                matdescra[3] = 'F'; /* One-based indexing */
                matdescra[4] = 'x'; /* No value */
                matdescra[5] = 'x'; /* No value */
                colsX = fpm[24];
                imem = N*(fpm[23]-1);
                mkl_zcsrmm(&transa,&N,&colsX,&N,&one,matdescra,val,cols,rows,rows+1,X+imem,&N,&zero,work+imem,&N);
                break;
            case 40:
                //!!!!!!!!!!!!! Perform multiplication B x[0:N-1][i:j]      !!!!!!!!!!!!!!!!!!!!
                //!!!!!!!!!!!!! and put result into work[0:N-1][i:j]        !!!!!!!!!!!!!!!!!!!!
                //!!!!!!!!!!!!! where i = fpm[23]-1, j = fpm[23]+fpm[24]-2  !!!!!!!!!!!!!!!!!!!!
                transa = 'N'; /* No transpose B */
                matdescra[0] = 'G'; /* general matrix */ 
                matdescra[1] = 'x'; /* No value */
                matdescra[2] = 'x'; /* No value */
                matdescra[3] = 'F'; /* one-based indexing */
                matdescra[4] = 'x'; /* No value */
                matdescra[5] = 'x'; /* No value */
                colsX = fpm[24];
                imem = N*(fpm[23]-1);
                mkl_zcsrmm(&transa,&N,&colsX,&N,&one,matdescra,valb,colsb,rowsb,rowsb+1,X+imem,&N,&zero,work+imem,&N);;
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
    //         with the help of ZFEAST_HRCI().
    //
    printf("  Number of eigenvalues found %d \n", M);
    printf("   Computed    |    Expected  \n");
    printf("   Eigenvalues |    Eigenvalues \n");
    eigabs = (double)0.0;
    for ( i=0; i<M; i++ )
    {
        R[i] = fabs(E[i]-Eig[i]);
        eigabs = max(eigabs, R[i]);
        printf("%.15e %.15e \n", E[i], Eig[i]); 
    }
    printf(" Max value of | computed eigenvalue -expected eigenvalues | %.15e \n", eigabs); 
    //
    //         Task 4.  Compute the maximum absolute value of the matrix
    //         Y=(X')*X-I  where X is the matrix of eigenvectors computed with
    //         the help of ZFEAST_HRCI.
    //         Call ZGEMM (BLAS Level 3 Routine) to compute (X')*X.
    //
    zgemm(&CGEMMC,&CGEMMN,&M,&M,&N,&one,X,&ldx,X,&ldx,&zero,Y, &ldy);

    //          Compute Y=Y-I.
    for ( i=0; i<M; i++ )
        Y[i][i].real = Y[i][i].real-(double)1.0;
    printf("*************************************************\n");
    printf("************** REPORT ***************************\n");
    printf("*************************************************\n");
    printf("# Search interval [Emin,Emax] %.15e %.15e\n",Emin,Emax);
    printf("# mode found/subspace %d %d \n",M,M0);
    printf("# iterations %d \n",loop);
    trace = (double)0.0;
    for ( i=0; i<M; i++ )
    {
        trace = trace+E[i];
    }
    printf("TRACE %.15e \n", trace);
    printf("Relative error on the Trace %.15e \n",epsout );
    printf("Eigenvalues/Residuals\n");
    for ( i=0; i<M; i++ )
    {
        printf("   %d  %.15e %.15e \n",i, E[i], res[i]);
    }
    smax = (double)0.0;
    for ( i=0; i<M; i++ )
    {
        for ( j=0; j<M; j++ )
        {
            t1 = sqrt(Y[i][j].imag*Y[i][j].imag+Y[i][j].real*Y[i][j].real);
            smax = max(smax, t1);
        }
    }
    printf( "Max of (conjugate transposed of X)*X-I %.15e \n", smax);
    return 0;
}
