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
!    vsNearbyInt  Example Program Text
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

  fA[0]=-2.0000;
  fA[1]=-1.5555;
  fA[2]=-1.1111;
  fA[3]=-0.6666;
  fA[4]=-0.2222;
  fA[5]=0.2222;
  fA[6]=0.6666;
  fA[7]=1.1111;
  fA[8]=1.5555;
  fA[9]=2.0000;
  fB[0]=-2.0000000000000000e+000;
  fB[1]=-2.0000000000000000e+000;
  fB[2]=-1.0000000000000000e+000;
  fB[3]=-1.0000000000000000e+000;
  fB[4]=0.0000000000000000e+000;
  fB[5]=0.0000000000000000e+000;
  fB[6]=1.0000000000000000e+000;
  fB[7]=1.0000000000000000e+000;
  fB[8]=2.0000000000000000e+000;
  fB[9]=2.0000000000000000e+000;

  vsNearbyInt(vec_len,fA,fBha0);

  vmsNearbyInt(vec_len,fA,fBep1,VML_EP);

  vmlSetMode(VML_EP);
  vsNearbyInt(vec_len,fA,fBep2);

  vmsNearbyInt(vec_len,fA,fBla1,VML_LA);

  vmlSetMode(VML_LA);
  vsNearbyInt(vec_len,fA,fBla2);

  vmsNearbyInt(vec_len,fA,fBha1,VML_HA);

  vmlSetMode(VML_HA);
  vsNearbyInt(vec_len,fA,fBha2);

  for(i=0;i<10;i++) {
    if(fBha0[i]!=fBha1[i] || fBha1[i]!=fBha2[i]) {
      printf("Error! Difference between vsNearbyInt and vmsNearbyInt in VML_HA mode detected.\n");
      return 1;
    }

    if(fBla1[i]!=fBla2[i]) {
      printf("Error! Difference between vsNearbyInt and vmsNearbyInt in VML_LA mode detected.\n");
      return 1;
    }

    if(fBep1[i]!=fBep2[i]) {
      printf("Error! Difference between vsNearbyInt and vmsNearbyInt in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vsNearbyInt test/example program\n\n");
  printf("           Argument                     vsNearbyInt\n");
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
