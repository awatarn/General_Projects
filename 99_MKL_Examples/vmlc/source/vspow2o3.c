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
!    vsPow2o3  Example Program Text
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
  fA[1]=1.6666;
  fA[2]=2.3333;
  fA[3]=3.0000;
  fA[4]=3.6666;
  fA[5]=4.3333;
  fA[6]=5.0000;
  fA[7]=5.6666;
  fA[8]=6.3333;
  fA[9]=7.0000;
  fB[0]=1.0000000000000000e+000;
  fB[1]=1.4056836164696351e+000;
  fB[2]=1.7591939986789722e+000;
  fB[3]=2.0800838230519041e+000;
  fB[4]=2.3778020028474529e+000;
  fB[5]=2.6579441157000709e+000;
  fB[6]=2.9240177382128660e+000;
  fB[7]=3.1784475544275574e+000;
  fB[8]=3.4231036179547671e+000;
  fB[9]=3.6593057100229713e+000;

  vsPow2o3(vec_len,fA,fBha0);

  vmsPow2o3(vec_len,fA,fBep1,VML_EP);

  vmlSetMode(VML_EP);
  vsPow2o3(vec_len,fA,fBep2);

  vmsPow2o3(vec_len,fA,fBla1,VML_LA);

  vmlSetMode(VML_LA);
  vsPow2o3(vec_len,fA,fBla2);

  vmsPow2o3(vec_len,fA,fBha1,VML_HA);

  vmlSetMode(VML_HA);
  vsPow2o3(vec_len,fA,fBha2);

  for(i=0;i<10;i++) {
    if(fBha0[i]!=fBha1[i] || fBha1[i]!=fBha2[i]) {
      printf("Error! Difference between vsPow2o3 and vmsPow2o3 in VML_HA mode detected.\n");
      return 1;
    }

    if(fBla1[i]!=fBla2[i]) {
      printf("Error! Difference between vsPow2o3 and vmsPow2o3 in VML_LA mode detected.\n");
      return 1;
    }

    if(fBep1[i]!=fBep2[i]) {
      printf("Error! Difference between vsPow2o3 and vmsPow2o3 in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vsPow2o3 test/example program\n\n");
  printf("           Argument                     vsPow2o3\n");
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
