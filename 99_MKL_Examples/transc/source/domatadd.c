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
!  Content:
!  mkl_domatadd - out-of-place transposition routine,
!  Example Program Text ( C Interface )
!******************************************************************************/
#include <mkl_trans.h>
#include "common_func.h"

int main(int argc, char *argv[])
{ 
  size_t n=3, m=5;/* rows, cols of source matrix  */
  double a[] = {
    1.,  2.,   3.,    4.,    50., 
    5.,  6.,   7.,    8.,    60.,
    9.,  10.,  11.,  12.,    70.
  }; /* source matrix A */
  double b[] = {     
    1.,  2.,   3.,    4.,    50., 
    5.,  6.,   7.,    8.,    40.,
    9.,  10.,  11.,  12.,    30.
  }; /* source matrix B */
  double dst[9]; /* destination matrix */

  printf("\nExample of using mkl_domatadd transposition\n");
  printf("INPUT DATA:\nSource matrix A:\n");
  print_matrix(n, m, 'd', a);

  printf("Source matrix B:\n");
  print_matrix(n, m, 'd', b);

  /*
  **  Addition of transposed sub-matrix(3,3) a and unchanged sub-matrix(3,3) b
  */
  mkl_domatadd('R'  /* row-major ordering */, 
               'T'  /* A will be transposed */, 
               'N'  /* no changes to B */, 
                3   /* rows */, 
                3   /* cols */, 
                1.  /* alpha */, 
                a   /* source matrix */, 
                5   /* lda */, 
                1.  /* beta */, 
                b   /* source matrix */, 
                5   /* ldb */, 
                dst /* destination matrix */, 
                3   /* ldc */); 
  /* New matrix: c =  { 
  **    2.,  7.,   12.,
  **    7.,  12.,  17.,
  **    12., 17.,  22.
  ** }
  */
  printf("OUTPUT DATA:\nDestination matrix - addition of transposed submatrix(3,3) of A and submatrix of B:\n");  
  print_matrix(3, 3, 'd', dst);
  /*
  **  Addition of transposed sub-matrices(3,3) a and b
  */
  mkl_domatadd('R'  /* row-major ordering */, 
               'T'  /* A will be transposed */, 
               'T'  /* B will be transposed */, 
                3   /* rows */, 
                3   /* cols */, 
                1.  /* alpha */, 
                a   /* source matrix */, 
                5   /* lda */, 
                1.  /* beta */, 
                b   /* source matrix */, 
                5   /* ldb */, 
                dst /* destination matrix */, 
                3   /* ldc */); 
  /* New matrix: c =  { 
  **    2., 10.,  18.,
  **    4., 12.,  20.,
  **    6., 14.,  22.
  **    }
  */
  printf("Destination matrix - Addition of transposed submatrices(3,3) of A and B:\n"); 
  print_matrix(3, 3, 'd', dst);
  
  return 0;
}
