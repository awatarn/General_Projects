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
!
!  Content: Intel(R) MKL RCI CG (Conjugate Gradient method) C example with
!           preconditioner but without user-defined stopping criteria
!
!*******************************************************************************/

#include <stdio.h>
#include "mkl_rci.h"
#include "mkl_blas.h"
#include "mkl_spblas.h"
#include "mkl_service.h"

/*---------------------------------------------------------------------------*/
/*  Example program for solving symmetric positive definite system of equations.*/
/*  Simplest case: simple preconditioning and no the user-defined stopping tests.*/
/*---------------------------------------------------------------------------*/
int main (void)
{
  /*---------------------------------------------------------------------------*/
  /* Define arrays for the upper triangle of the coefficient matrix and rhs vector */
  /* Compressed sparse row storage is used for sparse representation           */
  /*---------------------------------------------------------------------------*/
  MKL_INT n = 8, rci_request, expected_itercount = 8, i, j;
  MKL_INT nRhs = 2;
  MKL_INT itercount[2];
  double rhs[2 * 8];
  /* Fill all arrays containing matrix data. */
  MKL_INT ia[9] = { 1, 5, 8, 10, 12, 15, 17, 18, 19 };
  MKL_INT ja[18] = 
  { 1,    3,       6, 7,
       2, 3,    5,
          3,             8,
             4,       7,
                5, 6, 7,
                   6,    8,
                      7,
                         8
  };
  double a[18] = 
  { 7.E0,       1.E0,             2.E0, 7.E0,
         -4.E0, 8.E0,       2.E0,
                1.E0,                         5.E0,
                      7.E0,             9.E0,
                            5.E0, 1.E0, 5.E0,
                                 -1.E0,       5.E0,
                                       11.E0,
                                              5.E0
  };

  /*---------------------------------------------------------------------------*/
  /* Allocate storage for the solver ?par and temporary storage tmp            */
  /*---------------------------------------------------------------------------*/
  MKL_INT length = 128, method = 1;
  double expected_sol[2 * 8] =
  { 1.E0, 0.E0, 1.E0, 0.E0, 1.E0, 0.E0, 1.E0, 0.E0,
    0.E0, 2.E0, 0.E0, 2.E0, 0.E0, 2.E0, 0.E0, 2.E0
  };
  /*---------------------------------------------------------------------------*/
  /* Some additional variables to use with the RCI (P)CG solver                */
  /*---------------------------------------------------------------------------*/
  double solution[2 * 8];
  MKL_INT ipar[128 + 2 * 2];
  double euclidean_norm, dpar[128 + 2 * 2], tmp[8 * (3 + 2)];
  char tr = 'u';
  double eone = -1.E0;
  MKL_INT ione = 1;

  /*---------------------------------------------------------------------------*/
  /* Initialize the right hand side through matrix-vector product              */
  /*---------------------------------------------------------------------------*/
  mkl_dcsrsymv (&tr, &n, a, ia, ja, &expected_sol[0], &rhs[0]);
  mkl_dcsrsymv (&tr, &n, a, ia, ja, &expected_sol[n], &rhs[n]);
  /*---------------------------------------------------------------------------*/
  /* Initialize the initial guess                                              */
  /*---------------------------------------------------------------------------*/
  for (i = 0; i < n; i++)
    solution[i] = 1.E0;
  for (i = 0; i < n; i++)
    solution[n + i] = 2.E0;
  /*---------------------------------------------------------------------------*/
  /* Initialize the solver                                                     */
  /*---------------------------------------------------------------------------*/
  for (i = 0; i < (length + 2 * nRhs); i++)
    ipar[i] = 0;
  for (i = 0; i < (length + 2 * nRhs); i++)
    dpar[i] = 0.E0;

  dcgmrhs_init (&n, solution, &nRhs, rhs, &method, &rci_request, ipar, dpar, tmp);
  if (rci_request != 0)
    goto failure;
  /*---------------------------------------------------------------------------*/
  /* Set the desired parameters:                                               */
  /* Integer parameters:                                                       */
  /* set the maximal number of iterations to 100                               */
  /* LOGICAL parameters:                                                       */
  /* run the Preconditioned version of RCI (P)CG with preconditioner C_inverse */
  /* DOUBLE parameters                                                         */
  /* -                                                                         */
  /*---------------------------------------------------------------------------*/
  ipar[4] = 100;
  ipar[10] = 1;
  ipar[8] = 1;
  ipar[9] = 0;
  dpar[0] = 1.E-5;
  /*---------------------------------------------------------------------------*/
  /* Compute the solution by RCI (P)CG solver without preconditioning          */
  /* Reverse Communications starts here                                        */
  /*---------------------------------------------------------------------------*/

rci:dcgmrhs (&n, solution, &nRhs, rhs, &rci_request, ipar, dpar, tmp);
  /*---------------------------------------------------------------------------*/
  /* If rci_request=0, then the solution was found with the required precision */
  /*---------------------------------------------------------------------------*/
  if (rci_request == 0)
    goto getsln;
  /*---------------------------------------------------------------------------*/
  /* If rci_request=1, then compute the vector A*tmp[0]                        */
  /* and put the result in vector tmp[n]                                       */
  /*---------------------------------------------------------------------------*/
  if (rci_request == 1)
    {
      mkl_dcsrsymv (&tr, &n, a, ia, ja, tmp, &tmp[n]);
      goto rci;
    }
  /*---------------------------------------------------------------------------*/
  /* If rci_request=3, then compute apply the simplest preconditioning         */
  /* on vector tmp[(2+ipar[2])*n] and put the result in vector tmp[2*n]        */
  /*---------------------------------------------------------------------------*/
  if (rci_request == 3)
    {
      for (i = 0; i < n; i++)
        tmp[2 * n + i] = tmp[(2 + ipar[2]) * n + i];
      goto rci;
    }
  /*---------------------------------------------------------------------------*/
  /* If rci_request=anything else, then dcg subroutine failed                  */
  /* to compute the solution vector: solution[n]                               */
  /*---------------------------------------------------------------------------*/
  goto failure;
  /*---------------------------------------------------------------------------*/
  /* Reverse Communication ends here                                           */
  /* Get the current iteration number into itercount                           */
  /*---------------------------------------------------------------------------*/

getsln:dcgmrhs_get (&n, solution, &nRhs, rhs, &rci_request, ipar, dpar, tmp, itercount);

  /*---------------------------------------------------------------------------*/
  /* Print solution vector: solution[n] and number of iterations: itercount    */
  /*---------------------------------------------------------------------------*/
  printf ("The system has been solved\n");
  printf ("The following solution obtained\n");
  for (i = 0; i < n / 2; i++)
    printf ("%6.3f  ", solution[i]);
  printf ("\n");
  for (i = n / 2; i < n; i++)
    printf ("%6.3f  ", solution[i]);
  printf ("\n");
  for (i = 0; i < n / 2; i++)
    printf ("%6.3f  ", solution[n + i]);
  printf ("\n");
  for (i = n / 2; i < n; i++)
    printf ("%6.3f  ", solution[n + i]);

  printf ("\nExpected solution is\n");
  for (i = 0; i < n / 2; i++)
    {
      printf ("%6.3f  ", expected_sol[i]);
      expected_sol[i] -= solution[i];
    }
  printf ("\n");
  for (i = n / 2; i < n; i++)
    {
      printf ("%6.3f  ", expected_sol[i]);
      expected_sol[i] -= solution[i];
    }
  printf ("\n");
  for (i = 0; i < n / 2; i++)
    {
      printf ("%6.3f  ", expected_sol[n + i]);
      expected_sol[n + i] -= solution[n + i];
    }
  printf ("\n");
  for (i = n / 2; i < n; i++)
    {
      printf ("%6.3f  ", expected_sol[n + i]);
      expected_sol[n + i] -= solution[n + i];
    }
  printf ("\n");

  i = 1;
  j = n * nRhs;
  euclidean_norm = dnrm2 (&j, expected_sol, &i);

  /*-------------------------------------------------------------------------*/
  /* Release internal Intel(R) MKL memory that might be used for computations         */
  /* NOTE: It is important to call the routine below to avoid memory leaks   */
  /* unless you disable Intel(R) MKL Memory Manager                                   */
  /*-------------------------------------------------------------------------*/
  MKL_Free_Buffers ();

  if (euclidean_norm < 1.0e-12)
    {
      printf ("This example has successfully PASSED through all steps of computation!\n");
      return 0;
    }
  else
    {
      printf ("This example may have FAILED as the computed solution differs\n");
      printf ("much from the expected solution (Euclidean norm is %e).\n", euclidean_norm);
      return 1;
    }
  /*-------------------------------------------------------------------------*/
  /* Release internal Intel(R) MKL memory that might be used for computations         */
  /* NOTE: It is important to call the routine below to avoid memory leaks   */
  /* unless you disable Intel(R) MKL Memory Manager                                   */
  /*-------------------------------------------------------------------------*/
failure:printf ("This example FAILED as the solver has returned the ERROR code %d", rci_request);
  MKL_Free_Buffers ();
  return 1;
}
