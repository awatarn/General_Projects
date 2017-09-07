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
!    vdPow3o2  Example Program Text
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
  dA[1]=1.6666;
  dA[2]=2.3333;
  dA[3]=3.0000;
  dA[4]=3.6666;
  dA[5]=4.3333;
  dA[6]=5.0000;
  dA[7]=5.6666;
  dA[8]=6.3333;
  dA[9]=7.0000;
  dB[0]=1.0000000000000000e+000;
  dB[1]=2.1515283164058059e+000;
  dB[2]=3.5641491645323993e+000;
  dB[3]=5.1961524227066320e+000;
  dB[4]=7.0209406389953184e+000;
  dB[5]=9.0204485812534294e+000;
  dB[6]=1.1180339887498949e+001;
  dB[7]=1.3489126762555683e+001;
  dB[8]=1.5938413532941007e+001;
  dB[9]=1.8520259177452136e+001;

  vdPow3o2(vec_len,dA,dBha0);

  vmdPow3o2(vec_len,dA,dBep1,VML_EP);

  vmlSetMode(VML_EP);
  vdPow3o2(vec_len,dA,dBep2);

  vmdPow3o2(vec_len,dA,dBla1,VML_LA);

  vmlSetMode(VML_LA);
  vdPow3o2(vec_len,dA,dBla2);

  vmdPow3o2(vec_len,dA,dBha1,VML_HA);

  vmlSetMode(VML_HA);
  vdPow3o2(vec_len,dA,dBha2);

  for(i=0;i<10;i++) {
    if(dBha0[i]!=dBha1[i] || dBha1[i]!=dBha2[i]) {
      printf("Error! Difference between vdPow3o2 and vmdPow3o2 in VML_HA mode detected.\n");
      return 1;
    }

    if(dBla1[i]!=dBla2[i]) {
      printf("Error! Difference between vdPow3o2 and vmdPow3o2 in VML_LA mode detected.\n");
      return 1;
    }

    if(dBep1[i]!=dBep2[i]) {
      printf("Error! Difference between vdPow3o2 and vmdPow3o2 in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vdPow3o2 test/example program\n\n");
  printf("           Argument                     vdPow3o2\n");
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
