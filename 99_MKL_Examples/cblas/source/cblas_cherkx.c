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
!      C B L A S _ C H E R K  Example Program Text ( C Interface )
!******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "mkl.h"

#include "mkl_example.h"

int main(int argc, char *argv[])
{
      FILE *in_file;
      char *in_file_name;

      MKL_INT         n, k;
      MKL_INT         lda, ldc;
      MKL_INT         rmaxa, cmaxa, rmaxc, cmaxc;
      float           alpha, beta;
      MKL_Complex8   *a, *c;
      CBLAS_LAYOUT    layout;
      CBLAS_TRANSPOSE trans;
      CBLAS_UPLO      uplo;
      MKL_INT         ma, na, typeC;

      printf("\n     C B L A S _ C H E R K  EXAMPLE PROGRAM\n");

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
      if( GetIntegerParameters(in_file, &n, &k) != 2 ) {
          printf("\n ERROR of n, k reading\n");
          return 1;
      }
      if( GetScalarsS(in_file, &alpha, &beta) != 2 ) {
          printf("\n ERROR of alpha, beta reading\n");
          return 1;
      }
      if( GetCblasCharParameters(in_file, &uplo, &trans, &layout) != 3 ) {
          printf("\n ERROR of uplo, trans, layout reading\n");
          return 1;
      }

      if( trans == CblasNoTrans ) {
          rmaxa = n + 1;
          cmaxa = k;
          ma    = n;
          na    = k;
      } else {
          rmaxa = k + 1;
          cmaxa = n;
          ma    = k;
          na    = n;
      }
      rmaxc = n + 1;
      cmaxc = n;
      a = (MKL_Complex8 *)mkl_calloc(rmaxa*cmaxa, sizeof( MKL_Complex8 ), 64);
      c = (MKL_Complex8 *)mkl_calloc(rmaxc*cmaxc, sizeof( MKL_Complex8 ), 64);
      if( a == NULL || c == NULL ) {
          printf( "\n Can't allocate memory for arrays\n");
          return 1;
      }
      if (layout == CblasRowMajor) {
         lda=cmaxa;
         ldc=cmaxc;
      } else {
         lda=rmaxa;
         ldc=rmaxc;
      }
      if( GetArrayC(in_file, &layout, 0, &ma, &na, a, &lda) != 0 ) {
        printf("\n ERROR of array A reading\n");
        return 1;
      }
      if (uplo == CblasUpper) typeC = UPPER_MATRIX;
      else                    typeC = LOWER_MATRIX;
      if( GetArrayC(in_file, &layout, typeC, &n, &n, c, &ldc) != 0 ) {
        printf("\n ERROR of array C reading\n");
        return 1;
      }
      fclose(in_file);

/*       Print input data                                      */

      printf("\n     INPUT DATA");
      printf("\n       N=%d  K=%d", n, k);
      printf("\n       ALPHA =%4.1f  BETA =%4.1f", alpha, beta);
      PrintParameters("UPLO", uplo);
      PrintParameters("TRANS", trans);
      PrintParameters("LAYOUT", layout);
      PrintArrayC(&layout, FULLPRINT, GENERAL_MATRIX, &ma, &na, a, &lda, "A");
      PrintArrayC(&layout, FULLPRINT, typeC, &n, &n, c, &ldc, "C");

/*      Call CBLAS_CHERK subroutine ( C Interface )            */

      cblas_cherk(layout, uplo, trans, n, k, alpha, a, lda, beta, c, ldc);

/*       Print output data                                     */

      printf("\n\n     OUTPUT DATA");
      PrintArrayC(&layout, FULLPRINT, typeC, &n, &n, c, &ldc, "C");

      mkl_free(a);
      mkl_free(c);

      return 0;
}
