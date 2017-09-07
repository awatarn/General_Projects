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
!    vsPowx  Example Program Text
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
  fA[1]=0.8666;
  fA[2]=1.6333;
  fA[3]=2.4000;
  fA[4]=3.1666;
  fA[5]=3.9333;
  fA[6]=4.7000;
  fA[7]=5.4666;
  fA[8]=6.2333;
  fA[9]=7.0000;
  fB[0]=1.1660039110312152e-004;
  fB[1]=5.6940619267520864e-001;
  fB[2]=6.8873522741537876e+000;
  fB[3]=3.1295720312255717e+001;
  fB[4]=9.3107199918766597e+001;
  fB[5]=2.1845299866537277e+002;
  fB[6]=4.4011122728416541e+002;
  fB[7]=7.9737651355941364e+002;
  fB[8]=1.3361785535769964e+003;
  fB[9]=2.1087453418813361e+003;

  vsPowx(vec_len,fA,fA[5],fBha0);

  vmsPowx(vec_len,fA,fA[5],fBep1,VML_EP);

  vmlSetMode(VML_EP);
  vsPowx(vec_len,fA,fA[5],fBep2);

  vmsPowx(vec_len,fA,fA[5],fBla1,VML_LA);

  vmlSetMode(VML_LA);
  vsPowx(vec_len,fA,fA[5],fBla2);

  vmsPowx(vec_len,fA,fA[5],fBha1,VML_HA);

  vmlSetMode(VML_HA);
  vsPowx(vec_len,fA,fA[5],fBha2);

  for(i=0;i<10;i++) {
    if(fBha0[i]!=fBha1[i] || fBha1[i]!=fBha2[i]) {
      printf("Error! Difference between vsPowx and vmsPowx in VML_HA mode detected.\n");
      return 1;
    }

    if(fBla1[i]!=fBla2[i]) {
      printf("Error! Difference between vsPowx and vmsPowx in VML_LA mode detected.\n");
      return 1;
    }

    if(fBep1[i]!=fBep2[i]) {
      printf("Error! Difference between vsPowx and vmsPowx in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vsPowx test/example program\n\n");
  printf("           Argument                     vsPowx\n");
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
