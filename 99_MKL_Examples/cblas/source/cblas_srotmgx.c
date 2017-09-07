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
!      C B L A S _ S R O T M G   Example Program Text ( C Interface )
!******************************************************************************/

#include <stdio.h>

#include "mkl_example.h"

int main(int argc, char *argv[])
{
      FILE *in_file;
      char *in_file_name;

      float      param[5];
      float      sd1, sd2, sx1, sy1;

      param[0] = 0.0;
      param[1] = 0.0;
      param[2] = 0.0;
      param[3] = 0.0;
      param[4] = 0.0;

      printf("\n     C B L A S _ S R O T M G  EXAMPLE PROGRAM\n");

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
      if( GetScalarsS(in_file, &sd1, &sd2, &sx1, &sy1) != 4 ) {
          printf("\n ERROR of sd1, sd2, sx1, sy1 reading\n");
          return 1;
      }
      fclose(in_file);

/*       Print input data                                      */

      printf("\n     INPUT DATA");
      printf("\n       SD1=%5.2f  SD2=%5.2f  SX1=%5.2f  SY1=%5.2f",
               sd1, sd2, sx1, sy1);

/*      Call CBLAS_SROTMG subroutine ( C Interface )           */

      cblas_srotmg(&sd1, &sd2, &sx1, sy1, param);

/*       Print output data                                     */

      printf("\n\n     OUTPUT DATA");
      printf("\n       SD1=%5.2f  SD2=%5.2f  SX1=%5.2f",
               sd1, sd2, sx1);
      PrintVectorS(1, 5, param, 1, "PARAM");

      return 0;
}

