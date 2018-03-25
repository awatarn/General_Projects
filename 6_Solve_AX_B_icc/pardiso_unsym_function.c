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
*=
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
/* Example program to show the use of the "PARDISO" routine */
/* on unsymmetric linear systems */
/* -------------------------------------------------------------------- */
/* This program can be downloaded from the following site: */
/* www.pardiso-project.org */
/* */
/* (C) Olaf Schenk, Department of Computer Science, */
/* University of Basel, Switzerland. */
/* Email: olaf.schenk@unibas.ch */
/* -------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mkl_pardiso.h"
#include "mkl_types.h"
#include "mkl_spblas.h"

/* PARDISO VARIABLES */
MKL_INT iparm[64];
MKL_INT maxfct, mnum, phase, error, msglvl;
MKL_INT nrhs = 1;     /* Number of right hand sides. */
void *pt[64];
MKL_INT mtype = 11;       /* Real unsymmetric matrix */
double ddum;          /* Double dummy */
MKL_INT idum;         /* Integer dummy. */


/* INTERNAL FUNCTIONS */
void SetupPardiso();
/*MKL_INT Unsym_Solver(MKL_INT *un, double *ua, int *uNa, MKL_INT *uia, MKL_INT *uNia,
                     MKL_INT *uja, MKL_INT *uNja, double *ub, int *uNb,
                     double  *ux, int *uNx);*/
MKL_INT Unsym_Solver(MKL_INT *un, double *ua, MKL_INT *uia, MKL_INT *uja, double *ub, double  *ux);

MKL_INT main (void)
{
    /* Matrix data. */
    MKL_INT n = 5;
    MKL_INT ia[6] = { 0, 3, 5, 8, 11, 13};
    MKL_INT Nia = 6;
    MKL_INT ja[13] =
    { 0, 1,    3,
      0, 1,
            2, 3, 4,
      0,    2, 3,
         1,       4
    };
    MKL_INT Nja = 13;
    double a[13] =
    { 1.0,-1.0,     -3.0,
     -2.0, 5.0,
                4.0, 6.0, 4.0,
     -4.0,      2.0, 7.0,
           8.0,          -5.0
    };
    int Na = 13;
    /* Set right hand side to one. */
    double b[5], x[5];
    int Nb = 5, Nx = 5;
    int i;
    for ( i = 0; i < n; i++ )
    {
        b[i] = 1;
    }
    /* SOLVER */
    SetupPardiso();
//    Unsym_Solver(&n, &a[0], &Na, &ia[0], &Nia, &ja[0], &Nja, &b[0], &Nb, &x[0], &Nx);
    Unsym_Solver(&n, &a[0], &ia[0], &ja[0], &b[0], &x[0]);
    /* OUTPUT */
    printf("OUTPUT 1 x[i]: \n");
    for ( i = 0; i < n; i++ )
    {
        printf("x[%d] = %.8e\n",i,x[i]);
    }
    printf("\n");

    return 0;
}

void SetupPardiso(){
/*
  This function is for setting up control parameters for Pardiso solver.
  It should be called before Unsym_Solver(). Calling it once should be OKAY.
  msglvl : switch for statistical reports 0: disable, 1: enable
  For further information, please refer to Intel MKL refernce manual or visit
  https://software.intel.com/en-us/mkl-developer-reference-fortran-pardiso-iparm-parameter
  Last update: Sep 11, 2017.
  Authoer: A. Wisitsorasak
*/
  MKL_INT i;
  /* -------------------------------------------------------------------- */
  /* .. Setup Pardiso control parameters. */
  /* -------------------------------------------------------------------- */
      if(msglvl==1) printf("SETUP PARDISO CONTROL PARAMETERS: ");
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
      iparm[11] = 0;        /* Conjugate transposed/transpose solve */
      iparm[12] = 1;        /* Maximum weighted matching algorithm is switched-on (default for non-symmetric) */
      iparm[13] = 0;        /* Output: Number of perturbed pivots */
      iparm[14] = 0;        /* Not in use */
      iparm[15] = 0;        /* Not in use */
      iparm[16] = 0;        /* Not in use */
      iparm[17] = -1;        /* 0: No report; Output: Number of nonzeros in the factor LU */
      iparm[18] = -1;        /* 0: No report; Output: Mflops for LU factorization */
      iparm[19] = 0;        /* Output: Numbers of CG Iterations */
      iparm[34] = 1;        /* C-style indexing. Columns/rows indexing in input arrays ia, ja starts from 0.*/
      maxfct = 1;           /* Maximum number of numerical factorizations. */
      mnum = 1;             /* Which factorization to use. */
      msglvl = 0;           /* 0: silent mode; Print statistical information in file */
      error = 0;            /* Initialize error flag */

  /* -------------------------------------------------------------------- */
  /* .. Initialize the internal solver memory pointer. This is only */
  /* necessary for the FIRST call of the PARDISO solver. */
  /* -------------------------------------------------------------------- */
      for ( i = 0; i < 64; i++ ){
               pt[i] = 0;
      }
      if(msglvl==1) printf("DONE\n");
}


/*MKL_INT Unsym_Solver(MKL_INT *un, double *ua, int *uNa, MKL_INT *uia, MKL_INT *uNia,
                     MKL_INT *uja, MKL_INT *uNja, double *ub, int *uNb,
                     double  *ux, int *uNx){*/
MKL_INT Unsym_Solver(MKL_INT *un, double *ua, MKL_INT *uia, MKL_INT *uja, double *ub, double  *ux){
/* This function solves AX = B by Intel MKL Pardiso solver.
   Matrix A must be an unsymmetric matrix and each element must be given in CSR3 format.
   Prefix 'u' is meant for internal variables. The function is passed by reference.
   For further information, please refer to Intel MKL refernce manual or visit
   https://software.intel.com/en-us/mkl-developer-reference-fortran-pardiso-iparm-parameter
   Last update: Sep 11, 2017.
   Authoer: A. Wisitsorasak
*/
  int i;
  /*for(i=0;i<*un;i++){
     printf("ua[%d] = %.4e\n",i,ua[i]);
  }
  printf("\n");
  for(i=0;i<*uNia;i++){
     printf("uia[%d] = %d\n",i,uia[i]);
  }
  printf("\n");
  for(i=0;i<*uNja;i++){
     printf("uja[%d] = %d\n",i,uja[i]);
  }
  printf("\n");
  for(i=0;i<*uNb;i++){
     printf("ub[%d] = %.4e\n",i,ub[i]);
  }
  printf("\n");*/
  // SOLVE
/* -------------------------------------------------------------------- */
/* .. Reordering and Symbolic Factorization. This step also allocates */
/* all memory that is necessary for the factorization. */
/* -------------------------------------------------------------------- */
   phase = 11;
   PARDISO(pt, &maxfct, &mnum, &mtype, &phase,
            un, ua, uia, uja, &idum, &nrhs, iparm, &msglvl, &ddum, &ddum, &error);
   if( error != 0 ){
      printf ("\nERROR during symbolic factorization: %d", error);
      exit (1);
   }
   if(msglvl==1){
     printf("\nReordering completed ... ");
     printf("\nNumber of nonzeros in factors = %d", iparm[17]);
     printf("\nNumber of factorization MFLOPS = %d", iparm[18]);
     printf("\n\n");
   }
/* -------------------------------------------------------------------- */
/* .. Numerical factorization. */
/* -------------------------------------------------------------------- */
   phase = 22;
   PARDISO(pt, &maxfct, &mnum, &mtype, &phase,
            un, ua, uia, uja, &idum, &nrhs, iparm, &msglvl, &ddum, &ddum, &error);
   if ( error != 0 ) {
       printf("\nERROR during numerical factorization: %d", error);
       exit(2);
   }
   if(msglvl==1) printf("\nFactorization completed ... \n\n");
/* -------------------------------------------------------------------- */
/* .. Back substitution and iterative refinement. */
/* -------------------------------------------------------------------- */
   phase = 33;
   iparm[11] = 0;        /* 0: non-transposed solve */
   PARDISO (pt, &maxfct, &mnum, &mtype, &phase,
            un, ua, uia, uja, &idum, &nrhs, iparm, &msglvl, ub, ux, &error);
   if( error != 0 ){
     printf ("\nERROR during solution: %d", error);
     exit (3);
   }
   /*printf ("\nThe solution of the system is: ");
   for ( i = 0; i < *uNx; i++ )
   {
       printf ("\n ux [%d] = %f", i, ux[i]);
   }
   printf ("\n\n\n");*/
}
