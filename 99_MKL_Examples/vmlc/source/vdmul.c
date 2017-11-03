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
!    vdMul  Example Program Text
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

  dA[0]=-100.0000;
  dA[1]=-77.7777;
  dA[2]=-55.5555;
  dA[3]=-33.3333;
  dA[4]=-11.1111;
  dA[5]=11.1111;
  dA[6]=33.3333;
  dA[7]=55.5555;
  dA[8]=77.7777;
  dA[9]=100.0000;
  dB[0]=1.0000000000000000e+004;
  dB[1]=6.0493706172899992e+003;
  dB[2]=3.0864135802500005e+003;
  dB[3]=1.1111088888900001e+003;
  dB[4]=1.2345654321000001e+002;
  dB[5]=1.2345654321000001e+002;
  dB[6]=1.1111088888900001e+003;
  dB[7]=3.0864135802500005e+003;
  dB[8]=6.0493706172899992e+003;
  dB[9]=1.0000000000000000e+004;

  vdMul(vec_len,dA,dA,dBha0);

  vmdMul(vec_len,dA,dA,dBep1,VML_EP);

  vmlSetMode(VML_EP);
  vdMul(vec_len,dA,dA,dBep2);

  vmdMul(vec_len,dA,dA,dBla1,VML_LA);

  vmlSetMode(VML_LA);
  vdMul(vec_len,dA,dA,dBla2);

  vmdMul(vec_len,dA,dA,dBha1,VML_HA);

  vmlSetMode(VML_HA);
  vdMul(vec_len,dA,dA,dBha2);

  for(i=0;i<10;i++) {
    if(dBha0[i]!=dBha1[i] || dBha1[i]!=dBha2[i]) {
      printf("Error! Difference between vdMul and vmdMul in VML_HA mode detected.\n");
      return 1;
    }

    if(dBla1[i]!=dBla2[i]) {
      printf("Error! Difference between vdMul and vmdMul in VML_LA mode detected.\n");
      return 1;
    }

    if(dBep1[i]!=dBep2[i]) {
      printf("Error! Difference between vdMul and vmdMul in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vdMul test/example program\n\n");
  printf("                     Arguments                               vdMul\n");
  printf("===============================================================================\n");
  for(i=0;i<10;i++) {
    printf("% 25.14f % 25.14f % 25.14e\n",dA[i],dA[i],dBha0[i]);
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