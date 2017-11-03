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
!    vsExpm1  Example Program Text
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

  fA[0]=-17.0000;
  fA[1]=-13.1111;
  fA[2]=-9.2222;
  fA[3]=-5.3333;
  fA[4]=-1.4444;
  fA[5]=2.4444;
  fA[6]=6.3333;
  fA[7]=10.2222;
  fA[8]=14.1111;
  fA[9]=18.0000;
  fB[0]=-9.9999995860062285e-001;
  fB[1]=-9.9999797734630202e-001;
  fB[2]=-9.9990117899688591e-001;
  fB[3]=-9.9517188962066971e-001;
  fB[4]=-7.6411242224240106e-001;
  fB[5]=1.0523634174991830e+001;
  fB[6]=5.6201153347619049e+002;
  fB[7]=2.7506126448834904e+004;
  fB[8]=1.3439175517311485e+006;
  fB[9]=6.5659968137330510e+007;

  vsExpm1(vec_len,fA,fBha0);

  vmsExpm1(vec_len,fA,fBep1,VML_EP);

  vmlSetMode(VML_EP);
  vsExpm1(vec_len,fA,fBep2);

  vmsExpm1(vec_len,fA,fBla1,VML_LA);

  vmlSetMode(VML_LA);
  vsExpm1(vec_len,fA,fBla2);

  vmsExpm1(vec_len,fA,fBha1,VML_HA);

  vmlSetMode(VML_HA);
  vsExpm1(vec_len,fA,fBha2);

  for(i=0;i<10;i++) {
    if(fBha0[i]!=fBha1[i] || fBha1[i]!=fBha2[i]) {
      printf("Error! Difference between vsExpm1 and vmsExpm1 in VML_HA mode detected.\n");
      return 1;
    }

    if(fBla1[i]!=fBla2[i]) {
      printf("Error! Difference between vsExpm1 and vmsExpm1 in VML_LA mode detected.\n");
      return 1;
    }

    if(fBep1[i]!=fBep2[i]) {
      printf("Error! Difference between vsExpm1 and vmsExpm1 in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vsExpm1 test/example program\n\n");
  printf("           Argument                     vsExpm1\n");
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