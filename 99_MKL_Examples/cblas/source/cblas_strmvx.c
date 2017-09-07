/*******************************************************************************
* Copyright 1999-2017 Intel Corporation All Rights Reserved.
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
!      C B L A S _ S T R M V  Example Program Text ( C Interface )
!******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "mkl.h"

#include "mkl_example.h"

int main(int argc, char *argv[])
{
      FILE *in_file;
      char *in_file_name;

      MKL_INT          n, lda, incx;
      MKL_INT          rmaxa, cmaxa;
      float           *a, *x;
      CBLAS_LAYOUT     layout;
      CBLAS_UPLO       uplo;
      CBLAS_TRANSPOSE  trans;
      CBLAS_DIAG       diag;
      MKL_INT          len_x, typeA;

      printf("\n     C B L A S _ S T R M V  EXAMPLE PROGRAM\n");

/*       Get input parameters                                  */

      if( argc == 1 ) {
         printf("\n You must specify in_file data file as 1-st parameter");
         return 1;
      }
      in_file_name = argv[1];

/*       Get input data                                        */
      if( (in_file = fopen( in_file_name, "r" )) == NULL ) {
         printf("\n ERROR on OPEN '%s' with mode=\"r\"\n", in_file_name);
         return 1;
      }
      if( GetIntegerParameters(in_file, &n) != 1 ) {
          printf("\n ERROR of n reading\n");
          return 1;
      }
      if( GetIntegerParameters(in_file, &incx) != 1 ) {
          printf("\n ERROR of incx reading\n");
          return 1;
      }
      if( GetCblasCharParameters(in_file, &uplo, &trans, &diag, &layout) != 4 ) {
          printf("\n ERROR of uplo, trans, diag, layout reading\n");
          return 1;
      }

      rmaxa = n + 1;
      cmaxa = n;
      len_x = 1+(n-1)*abs(incx);
      a = (float *)mkl_calloc(rmaxa*cmaxa, sizeof(float), 64);
      x = (float *)mkl_calloc(len_x, sizeof(float), 64);
      if( a == NULL || x == NULL ) {
          printf("\n Can't allocate memory for arrays\n");
          return 1;
      }
      if( layout == CblasRowMajor )
         lda=cmaxa;
      else
         lda=rmaxa;

      if( GetVectorS(in_file, n, x, incx) != len_x ) {
        printf("\n ERROR of vector X reading\n");
        return 1;
      }
      if( uplo == CblasUpper ) typeA = UPPER_MATRIX;
      else                     typeA = LOWER_MATRIX;
      if( GetArrayS(in_file, &layout, typeA, &n, &n, a, &lda) != 0 ) {
        printf("\n ERROR of array A reading\n");
        return 1;
      }
      fclose(in_file);

/*       Print input data                                      */

      printf("\n     INPUT DATA");
      printf("\n       N=%d", n);
      PrintParameters("UPLO, DIAG", uplo, diag);
      PrintParameters("TRANS", trans);
      PrintParameters("LAYOUT", layout);
      PrintVectorS(FULLPRINT, n, x, incx, "X");
      PrintArrayS(&layout, FULLPRINT, typeA, &n, &n, a, &lda, "A");

/*      Call CBLAS_STRMV subroutine ( C Interface )            */

      cblas_strmv(layout, uplo, trans, diag, n, a, lda, x, incx);

/*       Print output data                                     */

      printf("\n\n     OUTPUT DATA");
      PrintVectorS(FULLPRINT, n, x, incx, "X");

      mkl_free(a);
      mkl_free(x);

      return 0;
}

