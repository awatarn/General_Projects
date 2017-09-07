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
!      C B L A S _ D D O T I  Example Program Text ( C Interface )
!******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "mkl.h"

#include "mkl_example.h"

int main(int argc, char *argv[])
{
      FILE *in_file;
      char *in_file_name;

      MKL_INT    n;
      MKL_INT   *indx;
      double    *x, *y;

      MKL_INT    indmax;
      double     res;

      printf("\n     C B L A S _ D D O T I  EXAMPLE PROGRAM\n");

/*       Get input parameters                                  */

      if( argc == 1 ) {
         printf("\n You must specify in_file data file as 1-st parameter");
         return 1;
      }
      in_file_name = argv[1];

/*       Get input data                                       */

      if( (in_file = fopen( in_file_name, "r" )) == NULL ) {
         printf("\n ERROR on OPEN '%s' with mode=\"r\"\n", in_file_name);
         return 1;
      }
      if( GetIntegerParameters(in_file, &n) != 1 ) {
          printf("\n ERROR of n reading\n");
          return 1;
      }

      x    = (double *)mkl_calloc(n, sizeof( double ), 64);
      indx = (MKL_INT *)mkl_calloc(n, sizeof( MKL_INT ), 64);
      if( x == NULL || indx == NULL ) {
          printf( "Can't allocate memory for arrays\n");
          return 1;
      }
      if( GetVectorD(in_file, n, x, 1) != n ) {
        printf("\n ERROR of vector X reading\n");
        return 1;
      }
      if( GetVectorI(in_file, n, indx) != n ) {
        printf("\n ERROR of vector INDX reading\n");
        return 1;
      }
      indmax = MaxValue(n, indx)+1;
      y    = (double *)mkl_calloc(indmax, sizeof( double ), 64);
      if( y == NULL ) {
          printf( "Can't allocate memory for arrays\n");
          return 1;
      }
      if( GetVectorD(in_file, indmax, y, 1) != indmax ) {
        printf("\n ERROR of vector Y reading\n");
        return 1;
      }
      fclose(in_file);

/*       Print input data                                      */

      printf("\n     INPUT DATA");
      printf("\n       N=%d", n);
      PrintVectorD(SHORTPRINT, n, x, 1, "X");
      PrintVectorI(n, indx, "INDX");
      PrintVectorD(SHORTPRINT, indmax, y, 1, "Y");

/*      Call CBLAS_DDOTI subroutine ( C Interface )            */

      res = cblas_ddoti(n, x, indx, y);

/*      Print output data                                      */

      printf("\n\n     OUTPUT DATA");
      printf("\n       DDOTI = %8.3f", res);

      mkl_free(x);
      mkl_free(indx);
      mkl_free(y);

      return 0;
}

