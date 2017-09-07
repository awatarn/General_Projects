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
!  mkl_dimatcopy - in-place transposition routine,
!  Example Program Text ( C Interface )
!******************************************************************************/
#include <mkl_trans.h>
#include "common_func.h"

int main(int argc, char *argv[])
{
  size_t n=4, m=6; /* rows, cols of source matrix */
  double src[]= {                       
    1.,  2.,   3.,  4.,  5.,  6.,
    7.,  8.,   9.,  10., 11., 12.,
    13., 14.,  15., 16., 17., 18.,
    19., 20.,  21., 22., 23., 24.
  }; /* source matrix */

  printf("\nExample of using mkl_dimatcopy transposition\n");
  printf("INPUT DATA:\n");
  printf("Source matrix:\n");
  print_matrix(n, m, 'd', src);
                     
  /*
  **  Submatrix(3,4) will be transposed and the rest of the matrix will be unchanged    
  */
  mkl_dimatcopy('R'  /* row-major ordering */, 
                'T'  /* matrix will be transposed */, 
                 3   /* rows */, 
                 4   /* cols */, 
                 1   /* scales the input matrix */, 
                 src /* source matrix */, 
                 6   /* src_lda */, 
                 6   /* dst_lda */);
  /* New matrix: src = { 
  **  1.,  7.,   13.,  4.,   5.,   6.,
  **  2.,  8.,   14.,  10.,  11.,  12.,
  **  3.,  9.,   15.,  16.,  17.,  18.,
  **  4.,  10.,  16.,  22.,  23.,  24.        
  ** }
  */
  /*
  **  destination matrix will replace the source matrix with using dst_lda
  */
  printf("\nOUTPUT DATA:\nDestination matrix:\n");
  print_matrix(n, m, 'd', src);

  return 0;
}
