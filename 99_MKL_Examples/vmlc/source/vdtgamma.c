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
!    vdTGamma  Example Program Text
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

  dA[0]=  .5;
  dA[1]= 3.5;
  dA[2]= 6.5;
  dA[3]= 9.5;
  dA[4]=12.5;
  dA[5]=15.5;
  dA[6]=18.5;
  dA[7]=21.5;
  dA[8]=24.5;
  dA[9]=27.5;
  dB[0]=1.7724538509055160e+00;
  dB[1]=3.3233509704478426e+00;
  dB[2]=2.8788527781504436e+02;
  dB[3]=1.1929246199460901e+05;
  dB[4]=1.3684336546556586e+08;
  dB[5]=3.3483860987355646e+11;
  dB[6]=1.4986120533153361e+15;
  dB[7]=1.1082798113786904e+19;
  dB[8]=1.2599063430729375e+23;
  dB[9]=2.0858851927622669e+27;

  vdTGamma(vec_len,dA,dBha0);

  vmdTGamma(vec_len,dA,dBep1,VML_EP);

  vmlSetMode(VML_EP);
  vdTGamma(vec_len,dA,dBep2);

  vmdTGamma(vec_len,dA,dBla1,VML_LA);

  vmlSetMode(VML_LA);
  vdTGamma(vec_len,dA,dBla2);

  vmdTGamma(vec_len,dA,dBha1,VML_HA);

  vmlSetMode(VML_HA);
  vdTGamma(vec_len,dA,dBha2);

  for(i=0;i<10;i++) {
    if(dBha0[i]!=dBha1[i] || dBha1[i]!=dBha2[i]) {
      printf("Error! Difference between vdTGamma and vmdTGamma in VML_HA mode detected.\n");
      return 1;
    }

    if(dBla1[i]!=dBla2[i]) {
      printf("Error! Difference between vdTGamma and vmdTGamma in VML_LA mode detected.\n");
      return 1;
    }

    if(dBep1[i]!=dBep2[i]) {
      printf("Error! Difference between vdTGamma and vmdTGamma in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vdTGamma test/example program\n\n");
  printf("           Argument                     vdTGamma\n");
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
