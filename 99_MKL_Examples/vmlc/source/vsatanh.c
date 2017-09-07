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
!    vsAtanh  Example Program Text
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

  fA[0]=0.7500;
  fA[1]=0.7666;
  fA[2]=0.7833;
  fA[3]=0.8000;
  fA[4]=0.8166;
  fA[5]=0.8333;
  fA[6]=0.8500;
  fA[7]=0.8666;
  fA[8]=0.8833;
  fA[9]=0.9000;
  fB[0]=9.7295507452765662e-001;
  fB[1]=1.0120292082224451e+000;
  fB[2]=1.0538534302966309e+000;
  fB[3]=1.0986123217818020e+000;
  fB[4]=1.1465262447609375e+000;
  fB[5]=1.1988385373069341e+000;
  fB[6]=1.2561528979046688e+000;
  fB[7]=1.3192607775232970e+000;
  fB[8]=1.3905871775267402e+000;
  fB[9]=1.4722193640997716e+000;

  vsAtanh(vec_len,fA,fBha0);

  vmsAtanh(vec_len,fA,fBep1,VML_EP);

  vmlSetMode(VML_EP);
  vsAtanh(vec_len,fA,fBep2);

  vmsAtanh(vec_len,fA,fBla1,VML_LA);

  vmlSetMode(VML_LA);
  vsAtanh(vec_len,fA,fBla2);

  vmsAtanh(vec_len,fA,fBha1,VML_HA);

  vmlSetMode(VML_HA);
  vsAtanh(vec_len,fA,fBha2);

  for(i=0;i<10;i++) {
    if(fBha0[i]!=fBha1[i] || fBha1[i]!=fBha2[i]) {
      printf("Error! Difference between vsAtanh and vmsAtanh in VML_HA mode detected.\n");
      return 1;
    }

    if(fBla1[i]!=fBla2[i]) {
      printf("Error! Difference between vsAtanh and vmsAtanh in VML_LA mode detected.\n");
      return 1;
    }

    if(fBep1[i]!=fBep2[i]) {
      printf("Error! Difference between vsAtanh and vmsAtanh in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vsAtanh test/example program\n\n");
  printf("           Argument                     vsAtanh\n");
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
