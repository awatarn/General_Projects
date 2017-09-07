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
!    mkl_zimatcopy - in-place transposition routine,
!    Example Program Text ( C Interface )
!******************************************************************************/
#include <mkl_trans.h>
#include "common_func.h"

int main(int argc, char *argv[])
{
  size_t n=4, m=6; /* rows, cols of source matrix */
  MKL_Complex16 alpha;
  MKL_Complex16 src[] = {
      { 1.,  2.},  {3.,  4.},   {5.,  6.},  {7.,  8.},  {9., 10.}, {11., 12.},
      {13., 14.}, {15., 16.},  {17., 18.}, {19., 20.}, {21., 22.}, {23., 24.},
      {25., 26.}, {27., 28.},  {29., 30.}, {31., 32.}, {33., 34.}, {35., 36.},
      {37., 38.}, {39., 40.},  {41., 42.}, {43., 44.}, {45., 46.}, {47., 48.}
  };/* source matrix */
  alpha.real = 1.;
  alpha.imag = 0.;

  printf("\nExample of using mkl_zimatcopy transposition\n");
  printf("INPUT DATA:\n");
  printf("Source matrix:\n");
  print_matrix(n, m, 'z', src);
                         
  /*
  **  Submatrix(3,4) will be transposed and the rest of the matrix will be unchanged    
  */
  mkl_zimatcopy('R'   /* row-major ordering */, 
                'T'   /* A will be transposed */, 
                3     /* rows */, 
                4     /* cols */, 
                alpha /* scales the input matrix */, 
                src   /* source matrix */, 
                6     /* src_lda */, 
                6     /* dst_lda */);
  /* New matrix: src = { 
  **     1.,  2., 13., 14., 25., 26.,  7.,  8.,  9., 10., 11., 12.,
  **     3.,  4., 15., 16., 27., 28., 19., 20., 21., 22., 23., 24.,
  **     5.,  6., 17., 18., 29., 30., 31., 32., 33., 34., 35., 36.,
  **     7.,  8., 19., 20., 31., 32., 43., 44., 45., 46., 47., 48.      
  **  }
  */
  printf("OUTPUT DATA:\nDestination matrix - submatrix(3,4) will be transposed and the rest of the matrix will be unchanged:\n");
  print_matrix(n, m, 'z', src);

  /*
  **  Submatrix(2,6) will be conjugated and the rest of the matrix will be unchanged    
  */
  mkl_zimatcopy('R'   /* row-major ordering */, 
                'R'   /* A will be conjugated */, 
                2     /* rows */, 
                6     /* cols */, 
                alpha /* scales the input matrix */, 
                src   /* source matrix */, 
                6     /* src_lda */, 
                6     /* dst_lda */);
  /* New matrix: src = { 
  **     1., -2., 13.,  -14., 25.,  -26.,  7.,  -8.,   9.,  -10., 11.,  -12.,
  **     3., -4., 15.,  -16., 27.,  -28., 19.,  -20., 21.,  -22., 23.,  -24.,
  **     5.,  6., 17.,  18.,  29.,  30.,  31.,  32.,  33.,  34.,  35.,  36.,
  **     7.,  8., 19.,  20.,  31.,  32.,  43.,  44.,  45.,  46.,  47.,  48.     
  **  }
  */
  /*
  **  destination matrix will replace the source matrix with using dst_lda 
  */
  printf("Destination matrix - submatrix(2,6) will be conjugated and the rest of the matrix will be unchanged:\n");
  print_matrix(n, m, 'z', src);

  /*
  **  Submatrix(2,6) will be conjugate transposed and the rest of the matrix will be unchanged    
  */
  mkl_zimatcopy('R'   /* row-major ordering */, 
                'C'   /* A will be conjugate transposed */, 
                2     /* rows */, 
                3     /* cols */, 
                alpha /* scales the input matrix */, 
                src   /* source matrix */, 
                6     /* src_lda */, 
                6     /* dst_lda */);
  /* New matrix: src = { 
  **     1.,   2.,  3.,   4.,   25.,  -26.,  7.,  -8.,   9.,  -10., 11.,  -12.,
  **    13.,  14.,  15.,  16.,  27.,  -28., 19.,  -20., 21.,  -22., 23.,  -24.,
  **    25.,  26.,  27.,  28.,  29.,  30.,  31.,  32.,  33.,  34.,  35.,  36.,
  **     7.,  8.,   19.,  20.,  31.,  32.,  43.,  44.,  45.,  46.,  47.,  48.     
  **  }
  */
  /*
  **  destination matrix will replace the source matrix with using dst_lda
  */
  printf("Destination matrix - submatrix(2,6) will be conjugate transposed and the rest of the matrix will be unchanged:\n");
  print_matrix(n, m, 'z', src);

  return 0;
}
