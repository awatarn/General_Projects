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
!    vdExp  Example Program Text
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

  dA[0]=-17.0000;
  dA[1]=-13.1111;
  dA[2]=-9.2222;
  dA[3]=-5.3333;
  dA[4]=-1.4444;
  dA[5]=2.4444;
  dA[6]=6.3333;
  dA[7]=10.2222;
  dA[8]=14.1111;
  dA[9]=18.0000;
  dB[0]=4.1399377187851668e-008;
  dB[1]=2.0226540961279020e-006;
  dB[2]=9.8821042017595656e-005;
  dB[3]=4.8281109281801239e-003;
  dB[4]=2.3588756664457899e-001;
  dB[5]=1.1523633344163436e+001;
  dB[6]=5.6301146947418135e+002;
  dB[7]=2.7507115619920700e+004;
  dB[8]=1.3439182871964260e+006;
  dB[9]=6.5659969137330510e+007;

  vdExp(vec_len,dA,dBha0);

  vmdExp(vec_len,dA,dBep1,VML_EP);

  vmlSetMode(VML_EP);
  vdExp(vec_len,dA,dBep2);

  vmdExp(vec_len,dA,dBla1,VML_LA);

  vmlSetMode(VML_LA);
  vdExp(vec_len,dA,dBla2);

  vmdExp(vec_len,dA,dBha1,VML_HA);

  vmlSetMode(VML_HA);
  vdExp(vec_len,dA,dBha2);

  for(i=0;i<10;i++) {
    if(dBha0[i]!=dBha1[i] || dBha1[i]!=dBha2[i]) {
      printf("Error! Difference between vdExp and vmdExp in VML_HA mode detected.\n");
      return 1;
    }

    if(dBla1[i]!=dBla2[i]) {
      printf("Error! Difference between vdExp and vmdExp in VML_LA mode detected.\n");
      return 1;
    }

    if(dBep1[i]!=dBep2[i]) {
      printf("Error! Difference between vdExp and vmdExp in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vdExp test/example program\n\n");
  printf("           Argument                     vdExp\n");
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
