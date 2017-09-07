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
!    vsErfc  Example Program Text
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

  fA[0]=1.0000;
  fA[1]=1.3333;
  fA[2]=1.6666;
  fA[3]=2.0000;
  fA[4]=2.3333;
  fA[5]=2.6666;
  fA[6]=3.0000;
  fA[7]=3.3333;
  fA[8]=3.6666;
  fA[9]=4.0000;
  fB[0]=1.5729920705028513e-001;
  fB[1]=5.9352797166239271e-002;
  fB[2]=1.8426803996428834e-002;
  fB[3]=4.6777349810472662e-003;
  fB[4]=9.6759040424297541e-004;
  fB[5]=1.6250224920156284e-004;
  fB[6]=2.2090496998585441e-005;
  fB[7]=2.4290277515690265e-006;
  fB[8]=2.1560327858950370e-007;
  fB[9]=1.5417257900280020e-008;

  vsErfc(vec_len,fA,fBha0);

  vmsErfc(vec_len,fA,fBep1,VML_EP);

  vmlSetMode(VML_EP);
  vsErfc(vec_len,fA,fBep2);

  vmsErfc(vec_len,fA,fBla1,VML_LA);

  vmlSetMode(VML_LA);
  vsErfc(vec_len,fA,fBla2);

  vmsErfc(vec_len,fA,fBha1,VML_HA);

  vmlSetMode(VML_HA);
  vsErfc(vec_len,fA,fBha2);

  for(i=0;i<10;i++) {
    if(fBha0[i]!=fBha1[i] || fBha1[i]!=fBha2[i]) {
      printf("Error! Difference between vsErfc and vmsErfc in VML_HA mode detected.\n");
      return 1;
    }

    if(fBla1[i]!=fBla2[i]) {
      printf("Error! Difference between vsErfc and vmsErfc in VML_LA mode detected.\n");
      return 1;
    }

    if(fBep1[i]!=fBep2[i]) {
      printf("Error! Difference between vsErfc and vmsErfc in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vsErfc test/example program\n\n");
  printf("           Argument                     vsErfc\n");
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
