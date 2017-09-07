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
!    vcAsinh  Example Program Text
!******************************************************************************/

#include <stdio.h>
#include "mkl_vml.h"

#include "_rms.h"

int main()
{
  MKL_Complex8 cA[10],cB[10];
  MKL_Complex8 cBha0[10],cBha1[10],cBha2[10];
  MKL_Complex8           cBla1[10],cBla2[10];
  MKL_Complex8           cBep1[10],cBep2[10];
  float CurRMS,MaxRMS=0.0;

  MKL_INT i=0,vec_len=10;

  cA[0].real=3.0000;cA[0].imag=10000.0000;
  cA[1].real=1113.7777;cA[1].imag=8889.2227;
  cA[2].real=2224.5554;cA[2].imag=7778.4443;
  cA[3].real=3335.3333;cA[3].imag=6667.6665;
  cA[4].real=4446.1113;cA[4].imag=5556.8887;
  cA[5].real=5556.8887;cA[5].imag=4446.1113;
  cA[6].real=6667.6665;cA[6].imag=3335.3333;
  cA[7].real=7778.4443;cA[7].imag=2224.5554;
  cA[8].real=8889.2227;cA[8].imag=1113.7777;
  cA[9].real=10000.0000;cA[9].imag=3.0000;
  cB[0].real=9.9034875950361272e+000;cB[0].imag=1.5704963207244873e+000;
  cB[1].real=9.7935305399540784e+000;cB[1].imag=1.4461505413055420e+000;
  cB[2].real=9.6915674216669974e+000;cB[2].imag=1.2922420501708984e+000;
  cB[3].real=9.6098341803762892e+000;cB[3].imag=1.1069687604904175e+000;
  cB[4].real=9.5633416449802731e+000;cB[4].imag=8.9598953723907471e-001;
  cB[5].real=9.5633416471460873e+000;cB[5].imag=6.7480677366256714e-001;
  cB[6].real=9.6098341857711080e+000;cB[6].imag=4.6382755041122437e-001;
  cB[7].real=9.6915674281509592e+000;cB[7].imag=2.7855432033538818e-001;
  cB[8].real=9.7935305459913469e+000;cB[8].imag=1.2464573234319687e-001;
  cB[9].real=9.9034876000361258e+000;cB[9].imag=2.9999998514540493e-004;

  vcAsinh(vec_len,cA,cBha0);

  vmcAsinh(vec_len,cA,cBep1,VML_EP);

  vmlSetMode(VML_EP);
  vcAsinh(vec_len,cA,cBep2);

  vmcAsinh(vec_len,cA,cBla1,VML_LA);

  vmlSetMode(VML_LA);
  vcAsinh(vec_len,cA,cBla2);

  vmcAsinh(vec_len,cA,cBha1,VML_HA);

  vmlSetMode(VML_HA);
  vcAsinh(vec_len,cA,cBha2);

  for(i=0;i<10;i++) {
    if(cBha0[i].real!=cBha1[i].real || cBha0[i].imag!=cBha1[i].imag || cBha1[i].real!=cBha2[i].real || cBha1[i].imag!=cBha2[i].imag) {
      printf("Error! Difference between vcAsinh and vmcAsinh in VML_HA mode detected.\n");
      return 1;
    }

    if(cBla1[i].real!=cBla2[i].real || cBla1[i].imag!=cBla2[i].imag) {
      printf("Error! Difference between vcAsinh and vmcAsinh in VML_LA mode detected.\n");
      return 1;
    }

    if(cBep1[i].real!=cBep2[i].real || cBep1[i].imag!=cBep2[i].imag) {
      printf("Error! Difference between vcAsinh and vmcAsinh in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vcAsinh test/example program\n\n");
  printf("           Argument                           vcAsinh\n");
  printf("===============================================================================\n");
  for(i=0;i<10;i++) {
    printf("   % .4f %+.4f*i      % .10f % +.10f*i\n",cA[i].real,cA[i].imag,cBha0[i].real,cBha0[i].imag);
    CurRMS=crelerr(cB[i],cBha0[i]);
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
