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
!    vdAsinh  Example Program Text
!******************************************************************************/

#include <stdio.h>
#include "mkl_vml.h"

#include "_rms.h"

int main()
{
  double dA[10],dB[10];
  double dBha0[10],dBha1[10],dBha2[10];
  double           dBla1[10],dBla2[10];
  double           dBep1[10],dBep2[10];
  float CurRMS,MaxRMS=0.0;

  MKL_INT i=0,vec_len=10;

  dA[0]=3.0000;
  dA[1]=1113.7777;
  dA[2]=2224.5555;
  dA[3]=3335.3333;
  dA[4]=4446.1111;
  dA[5]=5556.8888;
  dA[6]=6667.6666;
  dA[7]=7778.4444;
  dA[8]=8889.2222;
  dA[9]=10000.0000;
  dB[0]=1.8184464592320668e+000;
  dB[1]=7.7086602314986807e+000;
  dB[2]=8.4004596304405013e+000;
  dB[3]=8.8054750964190056e+000;
  dB[4]=9.0929322761725722e+000;
  dB[5]=9.3159408509385688e+000;
  dB[6]=9.4981724288039011e+000;
  dB[7]=9.6522588332858259e+000;
  dB[8]=9.7857420168405564e+000;
  dB[9]=9.9034875550361274e+000;

  vdAsinh(vec_len,dA,dBha0);

  vmdAsinh(vec_len,dA,dBep1,VML_EP);

  vmlSetMode(VML_EP);
  vdAsinh(vec_len,dA,dBep2);

  vmdAsinh(vec_len,dA,dBla1,VML_LA);

  vmlSetMode(VML_LA);
  vdAsinh(vec_len,dA,dBla2);

  vmdAsinh(vec_len,dA,dBha1,VML_HA);

  vmlSetMode(VML_HA);
  vdAsinh(vec_len,dA,dBha2);

  for(i=0;i<10;i++) {
    if(dBha0[i]!=dBha1[i] || dBha1[i]!=dBha2[i]) {
      printf("Error! Difference between vdAsinh and vmdAsinh in VML_HA mode detected.\n");
      return 1;
    }

    if(dBla1[i]!=dBla2[i]) {
      printf("Error! Difference between vdAsinh and vmdAsinh in VML_LA mode detected.\n");
      return 1;
    }

    if(dBep1[i]!=dBep2[i]) {
      printf("Error! Difference between vdAsinh and vmdAsinh in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vdAsinh test/example program\n\n");
  printf("           Argument                     vdAsinh\n");
  printf("===============================================================================\n");
  for(i=0;i<10;i++) {
    printf("% 25.14f % 25.14e\n",dA[i],dBha0[i]);
    CurRMS=drelerr(dB[i],dBha0[i]);
    if(CurRMS>MaxRMS) MaxRMS=CurRMS;
  }
  printf("\n");
  if(MaxRMS>=1e-14) {
    printf("Error! Relative accuracy is %.16f\n",MaxRMS);
    return 1;
  }
  else {
    printf("Relative accuracy is %.16f\n",MaxRMS);
  }

  return 0;
}
