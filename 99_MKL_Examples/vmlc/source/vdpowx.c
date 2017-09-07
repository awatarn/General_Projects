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
!    vdPowx  Example Program Text
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

  dA[0]=0.1000;
  dA[1]=0.8666;
  dA[2]=1.6333;
  dA[3]=2.4000;
  dA[4]=3.1666;
  dA[5]=3.9333;
  dA[6]=4.7000;
  dA[7]=5.4666;
  dA[8]=6.2333;
  dA[9]=7.0000;
  dB[0]=1.1660038918514482e-004;
  dB[1]=5.6940625356680863e-001;
  dB[2]=6.8873530949113730e+000;
  dB[3]=3.1295714919202130e+001;
  dB[4]=9.3107199233060683e+001;
  dB[5]=2.1845298918747244e+002;
  dB[6]=4.4011128506374024e+002;
  dB[7]=7.9737652246254356e+002;
  dB[8]=1.3361783325501383e+003;
  dB[9]=2.1087452667453094e+003;

  vdPowx(vec_len,dA,dA[5],dBha0);

  vmdPowx(vec_len,dA,dA[5],dBep1,VML_EP);

  vmlSetMode(VML_EP);
  vdPowx(vec_len,dA,dA[5],dBep2);

  vmdPowx(vec_len,dA,dA[5],dBla1,VML_LA);

  vmlSetMode(VML_LA);
  vdPowx(vec_len,dA,dA[5],dBla2);

  vmdPowx(vec_len,dA,dA[5],dBha1,VML_HA);

  vmlSetMode(VML_HA);
  vdPowx(vec_len,dA,dA[5],dBha2);

  for(i=0;i<10;i++) {
    if(dBha0[i]!=dBha1[i] || dBha1[i]!=dBha2[i]) {
      printf("Error! Difference between vdPowx and vmdPowx in VML_HA mode detected.\n");
      return 1;
    }

    if(dBla1[i]!=dBla2[i]) {
      printf("Error! Difference between vdPowx and vmdPowx in VML_LA mode detected.\n");
      return 1;
    }

    if(dBep1[i]!=dBep2[i]) {
      printf("Error! Difference between vdPowx and vmdPowx in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vdPowx test/example program\n\n");
  printf("           Argument                     vdPowx\n");
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
