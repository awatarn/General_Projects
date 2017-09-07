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
!    vsExpInt1  Example Program Text
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

  fA[0]=0.0099;
  fA[1]=1.1188;
  fA[2]=2.2277;
  fA[3]=3.3366;
  fA[4]=4.4455;
  fA[5]=5.5544;
  fA[6]=6.6633;
  fA[7]=7.7722;
  fA[8]=8.8811;
  fA[9]=10.9900;
  fB[0]=4.0478804136108807e+000;
  fB[1]=1.8040243730827092e-001;
  fB[2]=3.5823720740759158e-002;
  fB[3]=8.5395671171776873e-003;
  fB[4]=2.2125289768999900e-003;
  fB[5]=6.0177553854344804e-004;
  fB[6]=1.6900054139095165e-004;
  fB[7]=4.8555131001862730e-005;
  fB[8]=1.4190391394966996e-005;
  fB[9]=1.4155671423752270e-006;

  vsExpInt1(vec_len,fA,fBha0);

  vmsExpInt1(vec_len,fA,fBep1,VML_EP);

  vmlSetMode(VML_EP);
  vsExpInt1(vec_len,fA,fBep2);

  vmsExpInt1(vec_len,fA,fBla1,VML_LA);

  vmlSetMode(VML_LA);
  vsExpInt1(vec_len,fA,fBla2);

  vmsExpInt1(vec_len,fA,fBha1,VML_HA);

  vmlSetMode(VML_HA);
  vsExpInt1(vec_len,fA,fBha2);

  for(i=0;i<10;i++) {
    if(fBha0[i]!=fBha1[i] || fBha1[i]!=fBha2[i]) {
      printf("Error! Difference between vsExpInt1 and vmsExpInt1 in VML_HA mode detected.\n");
      return 1;
    }

    if(fBla1[i]!=fBla2[i]) {
      printf("Error! Difference between vsExpInt1 and vmsExpInt1 in VML_LA mode detected.\n");
      return 1;
    }

    if(fBep1[i]!=fBep2[i]) {
      printf("Error! Difference between vsExpInt1 and vmsExpInt1 in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vsExpInt1 test/example program\n\n");
  printf("           Argument                     vsExpInt1\n");
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
