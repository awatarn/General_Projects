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
!    vsLog1p  Example Program Text
!******************************************************************************/

#include <stdio.h>
#include "mkl_vml.h"

#include "_rms.h"

int main()
{
  float fA[10],fB[10];
  float fBha0[10],fBha1[10],fBha2[10];
  float           fBla1[10],fBla2[10];
  float           fBep1[10],fBep2[10];
  float CurRMS,MaxRMS=0.0;

  MKL_INT i=0,vec_len=10;

  fA[0]=0.1000;
  fA[1]=1111.2000;
  fA[2]=2222.2998;
  fA[3]=3333.3999;
  fA[4]=4444.5000;
  fA[5]=5555.6001;
  fA[6]=6666.7002;
  fA[7]=7777.7998;
  fA[8]=8888.9004;
  fA[9]=10000.0000;
  fB[0]=9.5310181158975882e-002;
  fB[1]=7.0140952708511746e+000;
  fB[2]=7.7067477697774001e+000;
  fB[3]=8.1120480028314894e+000;
  fB[4]=8.3996476275611940e+000;
  fB[5]=8.6227417069790988e+000;
  fB[6]=8.8050302811490937e+000;
  fB[7]=8.9591573385224201e+000;
  fB[8]=9.0926711237910727e+000;
  fB[9]=9.2104403669765169e+000;

  vsLog1p(vec_len,fA,fBha0);

  vmsLog1p(vec_len,fA,fBep1,VML_EP);

  vmlSetMode(VML_EP);
  vsLog1p(vec_len,fA,fBep2);

  vmsLog1p(vec_len,fA,fBla1,VML_LA);

  vmlSetMode(VML_LA);
  vsLog1p(vec_len,fA,fBla2);

  vmsLog1p(vec_len,fA,fBha1,VML_HA);

  vmlSetMode(VML_HA);
  vsLog1p(vec_len,fA,fBha2);

  for(i=0;i<10;i++) {
    if(fBha0[i]!=fBha1[i] || fBha1[i]!=fBha2[i]) {
      printf("Error! Difference between vsLog1p and vmsLog1p in VML_HA mode detected.\n");
      return 1;
    }

    if(fBla1[i]!=fBla2[i]) {
      printf("Error! Difference between vsLog1p and vmsLog1p in VML_LA mode detected.\n");
      return 1;
    }

    if(fBep1[i]!=fBep2[i]) {
      printf("Error! Difference between vsLog1p and vmsLog1p in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vsLog1p test/example program\n\n");
  printf("           Argument                     vsLog1p\n");
  printf("===============================================================================\n");
  for(i=0;i<10;i++) {
    printf("% 25.14f % 25.14e\n",fA[i],fBha0[i]);
    CurRMS=srelerr(fB[i],fBha0[i]);
    if(CurRMS>MaxRMS) MaxRMS=CurRMS;
  }
  printf("\n");
  if(MaxRMS>=1e-5) {
    printf("Error! Relative accuracy is %.16f\n",MaxRMS);
    return 1;
  }
  else {
    printf("Relative accuracy is %.16f\n",MaxRMS);
  }

  return 0;
}