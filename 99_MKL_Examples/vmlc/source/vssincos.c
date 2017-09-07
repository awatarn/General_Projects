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
!    vsSinCos  Example Program Text
!******************************************************************************/

#include <stdio.h>
#include "mkl_vml.h"

#include "_rms.h"

int main()
{
  float fA[10],fB[10],fC[10];
  float fBha0[10],fBha1[10],fBha2[10];
  float           fBla1[10],fBla2[10];
  float           fBep1[10],fBep2[10];
  float fCha0[10],fCha1[10],fCha2[10];
  float           fCla1[10],fCla2[10];
  float           fCep1[10],fCep2[10];
  float CurRMS,MaxRMS=0.0;

  MKL_INT i=0,vec_len=10;

  fA[0]=-10000.0000;
  fA[1]=-7777.7778;
  fA[2]=-5555.5557;
  fA[3]=-3333.3333;
  fA[4]=-1111.1111;
  fA[5]=1111.1111;
  fA[6]=3333.3333;
  fA[7]=5555.5557;
  fA[8]=7777.7778;
  fA[9]=10000.0000;
  fB[0]=3.0561438888825215e-001;
  fB[1]=7.2123131893817349e-001;
  fB[2]=-9.3904866376910323e-001;
  fB[3]=1.0326209316981849e-001;
  fB[4]=8.4827292823844636e-001;
  fB[5]=-8.4827292823844636e-001;
  fB[6]=-1.0326209316981849e-001;
  fB[7]=9.3904866376910323e-001;
  fB[8]=-7.2123131893817349e-001;
  fB[9]=-3.0561438888825215e-001;
  fC[0]=-9.5215536825901481e-001;
  fC[1]=6.9269429374198155e-001;
  fC[2]=3.4378424494653864e-001;
  fC[3]=-9.9465418116759940e-001;
  fC[4]=5.2955928772685279e-001;
  fC[5]=5.2955928772685279e-001;
  fC[6]=-9.9465418116759940e-001;
  fC[7]=3.4378424494653864e-001;
  fC[8]=6.9269429374198155e-001;
  fC[9]=-9.5215536825901481e-001;

  vsSinCos(vec_len,fA,fBha0,fCha0);

  vmsSinCos(vec_len,fA,fBep1,fCep1,VML_EP);

  vmlSetMode(VML_EP);
  vsSinCos(vec_len,fA,fBep2,fCep2);

  vmsSinCos(vec_len,fA,fBla1,fCla1,VML_LA);

  vmlSetMode(VML_LA);
  vsSinCos(vec_len,fA,fBla2,fCla2);

  vmsSinCos(vec_len,fA,fBha1,fCha1,VML_HA);

  vmlSetMode(VML_HA);
  vsSinCos(vec_len,fA,fBha2,fCha2);

  for(i=0;i<10;i++) {
    if(fBha0[i]!=fBha1[i] || fCha0[i]!=fCha1[i] || fBha1[i]!=fBha2[i] || fCha1[i]!=fCha2[i]) {
      printf("Error! Difference between vsSinCos and vmsSinCos in VML_HA mode detected.\n");
      return 1;
    }

    if(fBla1[i]!=fBla2[i] || fCla1[i]!=fCla2[i]) {
      printf("Error! Difference between vsSinCos and vmsSinCos in VML_LA mode detected.\n");
      return 1;
    }

    if(fBep1[i]!=fBep2[i] || fCep1[i]!=fCep2[i]) {
      printf("Error! Difference between vsSinCos and vmsSinCos in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vsSinCos test/example program\n\n");
  printf("           Argument                            vsSinCos\n");
  printf("===============================================================================\n");
  for(i=0;i<10;i++) {
    printf("% 25.14f % 25.14e % 25.14e\n",fA[i],fBha0[i],fCha0[i]);
    CurRMS=srelerr(fB[i],fBha0[i]);
    if(CurRMS>MaxRMS) MaxRMS=CurRMS;
    CurRMS=srelerr(fC[i],fCha0[i]);
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
