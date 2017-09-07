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
*   Content : Intel(R) MKL DSS C example
*
********************************************************************************
*/
/*
**
** Example program to solve complex unsymmetrical system of equations.
** The example also demonstrates how to solve transposed and conjugate transposed systems
**  ATx=b and AHx=b.
**
*/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include "mkl_dss.h"
#include "mkl_types.h"
/*
** Define the array and rhs vectors
*/
#define NROWS       5
#define NCOLS       5
#define NNONZEROS   9
#define NRHS        1
static const MKL_INT nRows = NROWS;
static const MKL_INT nCols = NCOLS;
static const MKL_INT nNonZeros = NNONZEROS;
static const MKL_INT nRhs = NRHS;
static MKL_INT rowIndex[NROWS + 1] = { 1, 3, 5, 7, 9, 10 };
static MKL_INT columns[NNONZEROS] = { 1, 2, 1, 2, 3, 4, 3, 4, 5 };
static double values[NNONZEROS * 2] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, -9, -8, -7, -6, -5, -4, -3, -2, -1 };
static _DOUBLE_PRECISION_t rhs[NCOLS * 2];
static _DOUBLE_PRECISION_t solValues[NROWS * 2] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

MKL_INT main ()
{
  MKL_INT i, j;
  /* Allocate storage for the solver handle and the right-hand side. */
  _MKL_DSS_HANDLE_t handle;
  _INTEGER_t error;
  _CHARACTER_t statIn[] = "determinant", *uplo;
  _DOUBLE_PRECISION_t statOut[5], eps = 1e-6;
  MKL_INT opt = MKL_DSS_DEFAULTS, opt1;
  MKL_INT sym = MKL_DSS_NON_SYMMETRIC_COMPLEX;
  MKL_INT type = MKL_DSS_INDEFINITE;
/* --------------------- */
/* Initialize the solver */
/* --------------------- */

  error = dss_create (handle, opt);
  if (error != MKL_DSS_SUCCESS)
    goto printError;
/* ------------------------------------------- */
/* Define the non-zero structure of the matrix */
/* ------------------------------------------- */
  error = dss_define_structure (handle, sym, rowIndex, nRows, nCols, columns, nNonZeros);
  if (error != MKL_DSS_SUCCESS)
    goto printError;
/* ------------------ */
/* Reorder the matrix */
/* ------------------ */
  error = dss_reorder (handle, opt, 0);
  if (error != MKL_DSS_SUCCESS)
    goto printError;
/* ------------------ */
/* Factor the matrix  */
/* ------------------ */
  error = dss_factor_complex (handle, type, values);
  if (error != MKL_DSS_SUCCESS)
    goto printError;
/* ------------------------ */
/* Get the solution vector for Ax=b, ATx=b and AHx=b and check correctness */
/* ------------------------ */
  for (i = 0; i < 3; i++)
    {
      if (i == 0)
        {
          uplo = "non-transposed";
          opt1 = MKL_DSS_DEFAULTS;
        }
      else if (i == 1)
        {
          uplo = "transposed";
          opt1 = MKL_DSS_TRANSPOSE_SOLVE;
        }
      else if (i == 2)
        {
          uplo = "conjugate transposed";
          opt1 = MKL_DSS_CONJUGATE_SOLVE;
        }

      printf ("\nSolving %s system...\n", uplo);

// Compute rhs respectively to uplo to have solution solValue
      mkl_zcsrgemv (uplo, &nRows, values, rowIndex, columns, solValues, rhs);

// Nullify solution on entry (for sure)
      for (j = 0; j < nCols * 2; j++)
        solValues[j] = 0.0;

// Apply trans, conjg or non-trans option, solve system
      opt |= opt1;
      error = dss_solve_complex (handle, opt, rhs, nRhs, solValues);
      if (error != MKL_DSS_SUCCESS)
        goto printError;
      opt &= ~opt1;

// Check solution vector: should be {0,1,2,3,4,5,6,7,8,9}
      for (j = 0; j < nCols * 2; j++)
      {
        if ((solValues[j] > j + eps) || (solValues[j] < j - eps))
          {
            printf ("Incorrect solution\n");
            error = 1000 + i;
            goto printError;
          }
      }
      printf ("Print solution array: ");
      for (j = 0; j < nCols * 2; j++)
        printf (" %g", solValues[j]);

      printf ("\n");
    }
/* -------------------------- */
/* Deallocate solver storage  */
/* -------------------------- */
  error = dss_delete (handle, opt);
  if (error != MKL_DSS_SUCCESS)
    goto printError;
/* ---------------------- */
/* Print solution vector  */
/* ---------------------- */
  printf ("\nExample successfully PASSED!\n");
  exit (0);
printError:
  printf ("Solver returned error code %d\n", error);
  exit (1);
}
