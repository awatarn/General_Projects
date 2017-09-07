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
!    vzAcosh  Example Program Text
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

  zA[0].real=3.5000;zA[0].imag=10000.0000;
  zA[1].real=1114.2222;zA[1].imag=8889.2777;
  zA[2].real=2224.9444;zA[2].imag=7778.5555;
  zA[3].real=3335.6666;zA[3].imag=6667.8333;
  zA[4].real=4446.3888;zA[4].imag=5557.1111;
  zA[5].real=5557.1111;zA[5].imag=4446.3888;
  zA[6].real=6667.8333;zA[6].imag=3335.6666;
  zA[7].real=7778.5555;zA[7].imag=2224.9444;
  zA[8].real=8889.2777;zA[8].imag=1114.2222;
  zA[9].real=10000.0000;zA[9].imag=3.5000;
  zB[0].real=9.9034876162861227e+000;zB[0].imag=1.5704463268109383e+000;
  zB[1].real=9.7935428119074004e+000;zB[1].imag=1.4461021328656807e+000;
  zB[2].real=9.6915938598109470e+000;zB[2].imag=1.2921995583383095e+000;
  zB[3].real=9.6098741976279989e+000;zB[3].imag=1.1069387861764741e+000;
  zB[4].real=9.5633904086913990e+000;zB[4].imag=8.9597859512086708e-001;
  zB[5].real=9.5633904065260058e+000;zB[5].imag=6.7481774130490502e-001;
  zB[6].real=9.6098741922340434e+000;zB[6].imag=4.6385754781665739e-001;
  zB[7].real=9.6915938533276709e+000;zB[7].imag=2.7859677249597142e-001;
  zB[8].real=9.7935428058704304e+000;zB[8].imag=1.2469419546677529e-001;
  zB[9].real=9.9034876112861259e+000;zB[9].imag=3.4999998745833399e-004;

  vzAcosh(vec_len,zA,zBha0);

  vmzAcosh(vec_len,zA,zBep1,VML_EP);

  vmlSetMode(VML_EP);
  vzAcosh(vec_len,zA,zBep2);

  vmzAcosh(vec_len,zA,zBla1,VML_LA);

  vmlSetMode(VML_LA);
  vzAcosh(vec_len,zA,zBla2);

  vmzAcosh(vec_len,zA,zBha1,VML_HA);

  vmlSetMode(VML_HA);
  vzAcosh(vec_len,zA,zBha2);

  for(i=0;i<10;i++) {
    if(zBha0[i].real!=zBha1[i].real || zBha0[i].imag!=zBha1[i].imag || zBha1[i].real!=zBha2[i].real || zBha1[i].imag!=zBha2[i].imag) {
      printf("Error! Difference between vzAcosh and vmzAcosh in VML_HA mode detected.\n");
      return 1;
    }

    if(zBla1[i].real!=zBla2[i].real || zBla1[i].imag!=zBla2[i].imag) {
      printf("Error! Difference between vzAcosh and vmzAcosh in VML_LA mode detected.\n");
      return 1;
    }

    if(zBep1[i].real!=zBep2[i].real || zBep1[i].imag!=zBep2[i].imag) {
      printf("Error! Difference between vzAcosh and vmzAcosh in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vzAcosh test/example program\n\n");
  printf("           Argument                           vzAcosh\n");
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
