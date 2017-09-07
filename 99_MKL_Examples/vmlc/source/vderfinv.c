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
!    vdErfInv  Example Program Text
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

  dA[0]=0.0100;
  dA[1]=0.1188;
  dA[2]=0.2277;
  dA[3]=0.3366;
  dA[4]=0.4455;
  dA[5]=0.5544;
  dA[6]=0.6633;
  dA[7]=0.7722;
  dA[8]=0.8811;
  dA[9]=0.9900;
  dB[0]=8.8625012809505979e-003;
  dB[1]=1.0567581831000064e-001;
  dB[2]=2.0461387202766912e-001;
  dB[3]=3.0774968941144704e-001;
  dB[4]=4.1792165882573973e-001;
  dB[5]=5.3936052169714166e-001;
  dB[6]=6.7932174635974640e-001;
  dB[7]=8.5280285439695180e-001;
  dB[8]=1.1026675051940427e+000;
  dB[9]=1.8213863677184494e+000;

  vdErfInv(vec_len,dA,dBha0);

  vmdErfInv(vec_len,dA,dBep1,VML_EP);

  vmlSetMode(VML_EP);
  vdErfInv(vec_len,dA,dBep2);

  vmdErfInv(vec_len,dA,dBla1,VML_LA);

  vmlSetMode(VML_LA);
  vdErfInv(vec_len,dA,dBla2);

  vmdErfInv(vec_len,dA,dBha1,VML_HA);

  vmlSetMode(VML_HA);
  vdErfInv(vec_len,dA,dBha2);

  for(i=0;i<10;i++) {
    if(dBha0[i]!=dBha1[i] || dBha1[i]!=dBha2[i]) {
      printf("Error! Difference between vdErfInv and vmdErfInv in VML_HA mode detected.\n");
      return 1;
    }

    if(dBla1[i]!=dBla2[i]) {
      printf("Error! Difference between vdErfInv and vmdErfInv in VML_LA mode detected.\n");
      return 1;
    }

    if(dBep1[i]!=dBep2[i]) {
      printf("Error! Difference between vdErfInv and vmdErfInv in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vdErfInv test/example program\n\n");
  printf("           Argument                     vdErfInv\n");
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
