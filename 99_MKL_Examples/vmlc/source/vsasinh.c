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
!    vsAsinh  Example Program Text
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

  fA[0]=3.0000;
  fA[1]=1113.7777;
  fA[2]=2224.5554;
  fA[3]=3335.3333;
  fA[4]=4446.1113;
  fA[5]=5556.8887;
  fA[6]=6667.6665;
  fA[7]=7778.4443;
  fA[8]=8889.2227;
  fA[9]=10000.0000;
  fB[0]=1.8184464592320668e+000;
  fB[1]=7.7086602404420574e+000;
  fB[2]=8.4004595944431433e+000;
  fB[3]=8.8054750820135883e+000;
  fB[4]=9.0929323274814546e+000;
  fB[5]=9.3159408278816027e+000;
  fB[6]=9.4981724143920001e+000;
  fB[7]=9.6522588250499251e+000;
  fB[8]=9.7857420681667548e+000;
  fB[9]=9.9034875550361274e+000;

  vsAsinh(vec_len,fA,fBha0);

  vmsAsinh(vec_len,fA,fBep1,VML_EP);

  vmlSetMode(VML_EP);
  vsAsinh(vec_len,fA,fBep2);

  vmsAsinh(vec_len,fA,fBla1,VML_LA);

  vmlSetMode(VML_LA);
  vsAsinh(vec_len,fA,fBla2);

  vmsAsinh(vec_len,fA,fBha1,VML_HA);

  vmlSetMode(VML_HA);
  vsAsinh(vec_len,fA,fBha2);

  for(i=0;i<10;i++) {
    if(fBha0[i]!=fBha1[i] || fBha1[i]!=fBha2[i]) {
      printf("Error! Difference between vsAsinh and vmsAsinh in VML_HA mode detected.\n");
      return 1;
    }

    if(fBla1[i]!=fBla2[i]) {
      printf("Error! Difference between vsAsinh and vmsAsinh in VML_LA mode detected.\n");
      return 1;
    }

    if(fBep1[i]!=fBep2[i]) {
      printf("Error! Difference between vsAsinh and vmsAsinh in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vsAsinh test/example program\n\n");
  printf("           Argument                     vsAsinh\n");
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
