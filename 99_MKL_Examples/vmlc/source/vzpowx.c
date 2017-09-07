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
!    vzPowx  Example Program Text
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

  zA[0].real=0.1000;zA[0].imag=7.0000;
  zA[1].real=0.8666;zA[1].imag=6.2333;
  zA[2].real=1.6333;zA[2].imag=5.4666;
  zA[3].real=2.4000;zA[3].imag=4.6999;
  zA[4].real=3.1666;zA[4].imag=3.9333;
  zA[5].real=3.9333;zA[5].imag=3.1666;
  zA[6].real=4.7000;zA[6].imag=2.3999;
  zA[7].real=5.4666;zA[7].imag=1.6333;
  zA[8].real=6.2333;zA[8].imag=0.8666;
  zA[9].real=7.0000;zA[9].imag=0.0999;
  zB[0].real=1.4660080920771602e+001;zB[0].imag=-4.2457663156400498e+000;
  zB[1].real=6.6546252673495720e+000;zB[1].imag=-1.3285048886353213e+001;
  zB[2].real=-7.0387482202786016e+000;zB[2].imag=-1.4767324650988847e+001;
  zB[3].real=-2.1117845706227051e+001;zB[3].imag=-3.4930900830543785e+000;
  zB[4].real=-2.4424383742451884e+001;zB[4].imag=2.4394003554591237e+001;
  zB[5].real=4.1042328112091866e+000;zB[5].imag=6.8100661769434140e+001;
  zB[6].real=1.0377465385856878e+002;zB[6].imag=1.1602928377093086e+002;
  zB[7].real=3.5030954960170573e+002;zB[7].imag=1.3713895026288438e+002;
  zB[8].real=8.9261595917122804e+002;zB[8].imag=7.6109202498160059e+001;
  zB[9].real=2.0121481862211938e+003;zB[9].imag=-1.3059786621885627e+002;

  vzPowx(vec_len,zA,zA[5],zBha0);

  vmzPowx(vec_len,zA,zA[5],zBep1,VML_EP);

  vmlSetMode(VML_EP);
  vzPowx(vec_len,zA,zA[5],zBep2);

  vmzPowx(vec_len,zA,zA[5],zBla1,VML_LA);

  vmlSetMode(VML_LA);
  vzPowx(vec_len,zA,zA[5],zBla2);

  vmzPowx(vec_len,zA,zA[5],zBha1,VML_HA);

  vmlSetMode(VML_HA);
  vzPowx(vec_len,zA,zA[5],zBha2);

  for(i=0;i<10;i++) {
    if(zBha0[i].real!=zBha1[i].real || zBha0[i].imag!=zBha1[i].imag || zBha1[i].real!=zBha2[i].real || zBha1[i].imag!=zBha2[i].imag) {
      printf("Error! Difference between vzPowx and vmzPowx in VML_HA mode detected.\n");
      return 1;
    }

    if(zBla1[i].real!=zBla2[i].real || zBla1[i].imag!=zBla2[i].imag) {
      printf("Error! Difference between vzPowx and vmzPowx in VML_LA mode detected.\n");
      return 1;
    }

    if(zBep1[i].real!=zBep2[i].real || zBep1[i].imag!=zBep2[i].imag) {
      printf("Error! Difference between vzPowx and vmzPowx in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vzPowx test/example program\n\n");
  printf("           Argument                           vzPowx\n");
  printf("===============================================================================\n");
  for(i=0;i<10;i++) {
    printf("   % .2f %+.2f*i   % .2f %+.2f*i      % .5f % +.5f*i\n",zA[i].real,zA[i].imag,zA[5].real,zA[5].imag,zBha0[i].real,zBha0[i].imag);
    CurRMS=zrelerr(zB[i],zBha0[i]);
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
