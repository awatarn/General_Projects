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
!    vzAsinh  Example Program Text
!******************************************************************************/

#include <stdio.h>
#include "mkl_vml.h"

#include "_rms.h"

int main()
{
  MKL_Complex16 zA[10],zB[10];
  MKL_Complex16 zBha0[10],zBha1[10],zBha2[10];
  MKL_Complex16           zBla1[10],zBla2[10];
  MKL_Complex16           zBep1[10],zBep2[10];
  float CurRMS,MaxRMS=0.0;

  MKL_INT i=0,vec_len=10;

  zA[0].real=3.0000;zA[0].imag=10000.0000;
  zA[1].real=1113.7777;zA[1].imag=8889.2222;
  zA[2].real=2224.5555;zA[2].imag=7778.4444;
  zA[3].real=3335.3333;zA[3].imag=6667.6666;
  zA[4].real=4446.1111;zA[4].imag=5556.8888;
  zA[5].real=5556.8888;zA[5].imag=4446.1111;
  zA[6].real=6667.6666;zA[6].imag=3335.3333;
  zA[7].real=7778.4444;zA[7].imag=2224.5555;
  zA[8].real=8889.2222;zA[8].imag=1113.7777;
  zA[9].real=10000.0000;zA[9].imag=3.0000;
  zB[0].real=9.9034875950361272e+000;zB[0].imag=1.5704963268023966e+000;
  zB[1].real=9.7935304892829596e+000;zB[1].imag=1.4461505920337059e+000;
  zB[2].real=9.6915674320018415e+000;zB[2].imag=1.2922419942501833e+000;
  zB[3].real=9.6098341947868935e+000;zB[3].imag=1.1069687573846247e+000;
  zB[4].real=9.5633416390116412e+000;zB[4].imag=8.9598953878147525e-001;
  zB[5].real=9.5633416411774572e+000;zB[5].imag=6.7480677838165393e-001;
  zB[6].real=9.6098342001817105e+000;zB[6].imag=4.6382756221178462e-001;
  zB[7].real=9.6915674384858015e+000;zB[7].imag=2.7855432850566603e-001;
  zB[8].real=9.7935304953202280e+000;zB[8].imag=1.2464573322417867e-001;
  zB[9].real=9.9034876000361258e+000;zB[9].imag=2.9999998950000077e-004;

  vzAsinh(vec_len,zA,zBha0);

  vmzAsinh(vec_len,zA,zBep1,VML_EP);

  vmlSetMode(VML_EP);
  vzAsinh(vec_len,zA,zBep2);

  vmzAsinh(vec_len,zA,zBla1,VML_LA);

  vmlSetMode(VML_LA);
  vzAsinh(vec_len,zA,zBla2);

  vmzAsinh(vec_len,zA,zBha1,VML_HA);

  vmlSetMode(VML_HA);
  vzAsinh(vec_len,zA,zBha2);

  for(i=0;i<10;i++) {
    if(zBha0[i].real!=zBha1[i].real || zBha0[i].imag!=zBha1[i].imag || zBha1[i].real!=zBha2[i].real || zBha1[i].imag!=zBha2[i].imag) {
      printf("Error! Difference between vzAsinh and vmzAsinh in VML_HA mode detected.\n");
      return 1;
    }

    if(zBla1[i].real!=zBla2[i].real || zBla1[i].imag!=zBla2[i].imag) {
      printf("Error! Difference between vzAsinh and vmzAsinh in VML_LA mode detected.\n");
      return 1;
    }

    if(zBep1[i].real!=zBep2[i].real || zBep1[i].imag!=zBep2[i].imag) {
      printf("Error! Difference between vzAsinh and vmzAsinh in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vzAsinh test/example program\n\n");
  printf("           Argument                           vzAsinh\n");
  printf("===============================================================================\n");
  for(i=0;i<10;i++) {
    printf("   % .4f %+.4f*i      % .10f % +.10f*i\n",zA[i].real,zA[i].imag,zBha0[i].real,zBha0[i].imag);
    CurRMS=zrelerr(zB[i],zBha0[i]);
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
