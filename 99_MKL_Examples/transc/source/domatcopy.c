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
!    mkl_domatcopy - out-of-place transposition routine,
!    Example Program Text ( C Interface )
!******************************************************************************/
#include <mkl_trans.h>
#include "common_func.h"

int main(int argc, char *argv[])
{ 
  size_t n=3, m=5;
  double src[] = { 
    1.,   2.,   3.,   4.,   5.,
    6.,   7.,   8.,   9.,   10.,
    11.,  12.,  13.,  14.,  15.
  }; /* source matrix */
  double dst[8]; /* destination matrix */
  size_t src_stride = 5;
  size_t dst_stride = 2;

  printf("\nThis is example of using mkl_domatcopy\n");

  printf("INPUT DATA:\nSource matrix:\n");
  print_matrix(n, m, 'd', src);

  /*
  **  Source submatrix(2,4) a will be transposed 
  */
  mkl_domatcopy('R'        /* row-major ordering */,
                'T'        /* A will be transposed */,
                2          /* rows */,
                4          /* cols */,
                1.         /* scales the input matrix */,
                src        /* source matrix */,
                src_stride /* src_stride */,
                dst        /* destination matrix */,
                dst_stride /* dst_stride */);
  /*  New matrix: src = { 
  **      1,  6,
  **      2,  7,
  **      3,  8,
  **      4,  9,
  **    }
  */
  printf("OUTPUT DATA:\nDestination matrix:\n");
  print_matrix(4, 2, 'd', dst);
  
  return 0;
}
