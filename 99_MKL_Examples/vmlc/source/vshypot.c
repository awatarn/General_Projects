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
!    vsHypot  Example Program Text
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

  fA[0]=1.0009;
  fA[1]=1112.0009;
  fA[2]=2223.0007;
  fA[3]=3334.0005;
  fA[4]=4445.0005;
  fA[5]=5556.0005;
  fA[6]=6667.0005;
  fA[7]=7778.0000;
  fA[8]=8889.0000;
  fA[9]=10000.0000;
  fB[0]=1.4154863971981240e+000;
  fB[1]=1.5726066897933222e+003;
  fB[2]=3.1437977849563395e+003;
  fB[3]=4.7149887074858652e+003;
  fB[4]=6.2861799752823736e+003;
  fB[5]=7.8573712430788819e+003;
  fB[6]=9.4285625108753902e+003;
  fB[7]=1.0999753088137933e+004;
  fB[8]=1.2570944355934442e+004;
  fB[9]=1.4142135623730950e+004;

  vsHypot(vec_len,fA,fA,fBha0);

  vmsHypot(vec_len,fA,fA,fBep1,VML_EP);

  vmlSetMode(VML_EP);
  vsHypot(vec_len,fA,fA,fBep2);

  vmsHypot(vec_len,fA,fA,fBla1,VML_LA);

  vmlSetMode(VML_LA);
  vsHypot(vec_len,fA,fA,fBla2);

  vmsHypot(vec_len,fA,fA,fBha1,VML_HA);

  vmlSetMode(VML_HA);
  vsHypot(vec_len,fA,fA,fBha2);

  for(i=0;i<10;i++) {
    if(fBha0[i]!=fBha1[i] || fBha1[i]!=fBha2[i]) {
      printf("Error! Difference between vsHypot and vmsHypot in VML_HA mode detected.\n");
      return 1;
    }

    if(fBla1[i]!=fBla2[i]) {
      printf("Error! Difference between vsHypot and vmsHypot in VML_LA mode detected.\n");
      return 1;
    }

    if(fBep1[i]!=fBep2[i]) {
      printf("Error! Difference between vsHypot and vmsHypot in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vsHypot test/example program\n\n");
  printf("                     Arguments                               vsHypot\n");
  printf("===============================================================================\n");
  for(i=0;i<10;i++) {
    printf("% 25.14f % 25.14f % 25.14e\n",fA[i],fA[i],fBha0[i]);
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
