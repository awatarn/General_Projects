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
!    vdErfc  Example Program Text
!******************************************************************************/

#include <stdio.h>
#include "mkl_vml.h"

#include "_rms.h"

int main()
{
  double dA[10],dB[10];
  double dBha0[10],dBha1[10],dBha2[10];
  double           dBla1[10],dBla2[10];
  double           dBep1[10],dBep2[10];
  float CurRMS,MaxRMS=0.0;

  MKL_INT i=0,vec_len=10;

  dA[0]=1.0000;
  dA[1]=1.3333;
  dA[2]=1.6666;
  dA[3]=2.0000;
  dA[4]=2.3333;
  dA[5]=2.6666;
  dA[6]=3.0000;
  dA[7]=3.3333;
  dA[8]=3.6666;
  dA[9]=4.0000;
  dB[0]=1.5729920705028513e-001;
  dB[1]=5.9352796111263074e-002;
  dB[2]=1.8426803220117201e-002;
  dB[3]=4.6777349810472662e-003;
  dB[4]=9.6759095849420300e-004;
  dB[5]=1.6250223901210115e-004;
  dB[6]=2.2090496998585441e-005;
  dB[7]=2.4290296690607724e-006;
  dB[8]=2.1560326048955131e-007;
  dB[9]=1.5417257900280020e-008;

  vdErfc(vec_len,dA,dBha0);

  vmdErfc(vec_len,dA,dBep1,VML_EP);

  vmlSetMode(VML_EP);
  vdErfc(vec_len,dA,dBep2);

  vmdErfc(vec_len,dA,dBla1,VML_LA);

  vmlSetMode(VML_LA);
  vdErfc(vec_len,dA,dBla2);

  vmdErfc(vec_len,dA,dBha1,VML_HA);

  vmlSetMode(VML_HA);
  vdErfc(vec_len,dA,dBha2);

  for(i=0;i<10;i++) {
    if(dBha0[i]!=dBha1[i] || dBha1[i]!=dBha2[i]) {
      printf("Error! Difference between vdErfc and vmdErfc in VML_HA mode detected.\n");
      return 1;
    }

    if(dBla1[i]!=dBla2[i]) {
      printf("Error! Difference between vdErfc and vmdErfc in VML_LA mode detected.\n");
      return 1;
    }

    if(dBep1[i]!=dBep2[i]) {
      printf("Error! Difference between vdErfc and vmdErfc in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vdErfc test/example program\n\n");
  printf("           Argument                     vdErfc\n");
  printf("===============================================================================\n");
  for(i=0;i<10;i++) {
    printf("% 25.14f % 25.14e\n",dA[i],dBha0[i]);
    CurRMS=drelerr(dB[i],dBha0[i]);
    if(CurRMS>MaxRMS) MaxRMS=CurRMS;
  }
  printf("\n");
  if(MaxRMS>=1e-14) {
    printf("Error! Relative accuracy is %.16f\n",MaxRMS);
    return 1;
  }
  else {
    printf("Relative accuracy is %.16f\n",MaxRMS);
  }

  return 0;
}
