/*******************************************************************************
* Copyright 2001-2017 Intel Corporation All Rights Reserved.
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
!    vzUnpackI  Example Program Text
!******************************************************************************/

#include <stdio.h>
#include "mkl_vml.h"

int main()
{
  MKL_Complex16 zA[10],zB1[10],zB2[10];

  MKL_INT i=0,vec_len=10,incra=3;

  for(i=0;i<vec_len;i++) {
    zA[i].real=(double)i+1.0;zA[i].imag=(double)i+1.0;
    zB1[i].real=0.0;zB1[i].imag=0.0;
    zB2[i].real=0.0;zB2[i].imag=0.0;
  }

  vec_len=vec_len/incra+1;
  vzPackI(vec_len,zA,incra,zB1);
  vzUnpackI(vec_len,zB1,zB2,incra);

  printf("vzUnpackI test/example program\n\n");
  printf("     Before packing             After packing          After Unpacking\n");
  printf("===============================================================================\n");
  for(i=0;i<10;i++) {
    printf("   %.4f + %.4f*i         %.4f + %.4f*i         %.4f + %.4f*i\n",zA[i].real,zA[i].imag,zB1[i].real,zB1[i].imag,zB2[i].real,zB2[i].imag);
  }

  return 0;
}
