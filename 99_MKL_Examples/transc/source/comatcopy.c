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
!  mkl_comatcopy - out-of-place transposition routine,
!  Example Program Text ( C Interface )
!******************************************************************************/
#include <mkl_trans.h>
#include "common_func.h"

int main(int argc, char *argv[])
{                                             
  size_t n=4, m=6;
  MKL_Complex8 alpha;
  MKL_Complex8 src[] = {
      { 1.,  2.},  {3.,  4.},   {5.,  6.},  {7.,  8.},  {9., 10.}, {11., 12.},
      {13., 14.}, {15., 16.},  {17., 18.}, {19., 20.}, {21., 22.}, {23., 24.},
      {25., 26.}, {27., 28.},  {29., 30.}, {31., 32.}, {33., 34.}, {35., 36.},
      {37., 38.}, {39., 40.},  {41., 42.}, {43., 44.}, {45., 46.}, {47., 48.}
  };/* source matrix */
  MKL_Complex8 dst[12]; /* destination matrix */
  alpha.real = 1.;
  alpha.imag = 0.;
  
  printf("\nThis is example of using mkl_comatcopy\n");
  printf("INPUT DATA:\nSource matrix:\n");
  print_matrix(n, m, 'c', src);

  /*
  **  Source submatrix(3,4) a will be transposed and multiplied by alpha    
  */
  mkl_comatcopy('R'    /* row-major ordering */, 
                'T'    /* matrix will be transposed */, 
                 3     /* rows */, 
                 4     /* cols */, 
                 alpha /* scales the input matrix */, 
                 src   /* source matrix */, 
                 6     /* src_stride */, 
                 dst   /* destination matrix */, 
                 3     /* dst_stride */);
  /*  New matrix: src = { 
  **    1., 2.,  13., 14.,    25., 26.,
  **    3., 4.,  15., 16.,    27., 28.,
  **    5., 6.,  17., 18.,    29., 30.,
  **    7., 8.,  19., 20.,    31., 32.
  **  }
  */
  printf("OUTPUT DATA:\nDestination matrix - source submatrix(3,4) a will be transposed:\n");
  print_matrix(4, 3, 'c', dst);

  /*  
  **  Source submatrix(3,4) a will be conjugate transposed    
  */
  mkl_comatcopy('R'    /* row-major ordering */, 
                'C'    /* matrix will be conjugate transposed */, 
                 4     /* rows */, 
                 3     /* cols */, 
                 alpha /* scales the input matrix */, 
                 src   /* source matrix */, 
                 6     /* src_stride */, 
                 dst   /* destination matrix */, 
                 4     /* dst_stride */);
  /* New matrix: src = { 
  **  1., -2.,  13., -14.,  25., -26.,  37., -38.,
  **  3., -4.,  15., -16.,  27., -28.,  39., -40.,
  **  5., -6.,  17., -18.,  29., -30.,  41., -42.
  ** }
  */
  printf("Destination matrix - source submatrix(4,3) a will be conjugate transposed:\n");
  print_matrix(3, 4, 'c', dst);

  /* 
  **  Source submatrix(3,4) a will be conjugated    
  */
  mkl_comatcopy('R'   /* row-major ordering */, 
                'R'   /* A will be conjugated */, 
                3     /* rows */, 
                4     /* cols */, 
                alpha /* scales the input matrix */, 
                src   /* source matrix */, 
                6     /* src_stride */, 
                dst   /* destination matrix */, 
                4     /* dst_stride */);
  /*  New matrix: src = {  
  **    1., - 2.,     3., - 4.,    5., - 6.,    7., - 8., 
  **   13., -14.,    15., -16.,    17., -18.,   19., -20.,  
  **   25., -26.,    27., -28.,    29., -30.,   31., -32.
  **  }
  */
  printf("Destination matrix - source submatrix(3,4) a will be conjugated:\n");
  print_matrix(3, 4, 'c', dst);
  
  return 0;
}
