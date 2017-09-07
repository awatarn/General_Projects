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
!    vsUnpackM  Example Program Text
!******************************************************************************/

#include <stdio.h>
#include "mkl_vml.h"

int main()
{
  float fA[10],fB1[10],fB2[10];

  MKL_INT i=0,vec_len=10,ma[10];

  for(i=0;i<vec_len;i++) {
    fA[i]=(float)i+1.0;
    fB1[i]=0.0;
    fB2[i]=0.0;
    ma[i]=i&1;
  }

  vsPackM(vec_len,fA,ma,fB1);
  vsUnpackM(vec_len,fB1,fB2,ma);

  printf("vsUnpackM test/example program\n\n");
  printf("           Before packing             After packing          After Unpacking\n");
  printf("===============================================================================\n");
  for(i=0;i<10;i++) {
    printf("% 25.14f ",fA[i]);
    printf("% 25.14f ",fB1[i]);
    printf("% 25.14f",fB2[i]);
    printf("\n");
  }

  return 0;
}
