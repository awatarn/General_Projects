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
!    vzTanh  Example Program Text
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

  zA[0].real=-10.0000;zA[0].imag=10.0000;
  zA[1].real=-7.7777;zA[1].imag=7.7777;
  zA[2].real=-5.5555;zA[2].imag=5.5555;
  zA[3].real=-3.3333;zA[3].imag=3.3333;
  zA[4].real=-1.1111;zA[4].imag=1.1111;
  zA[5].real=1.1111;zA[5].imag=-1.1111;
  zA[6].real=3.3333;zA[6].imag=-3.3333;
  zA[7].real=5.5555;zA[7].imag=-5.5555;
  zA[8].real=7.7777;zA[8].imag=-7.7777;
  zA[9].real=10.0000;zA[9].imag=-10.0000;
  zB[0].real=-9.9999999831776032e-001;zB[0].imag=3.7634408149196452e-009;
  zB[1].real=-1.0000003470018055e+000;zB[1].imag=5.3354333100097443e-008;
  zB[2].real=-9.9999655668583243e-001;zB[2].imag=-2.9694976884679805e-005;
  zB[3].real=-9.9764171139512925e-001;zB[3].imag=9.4997704948177541e-004;
  zB[4].real=-1.1225926239345323e+000;zB[4].imag=1.9578899279779802e-001;
  zB[5].real=1.1225926239345323e+000;zB[5].imag=-1.9578899279779802e-001;
  zB[6].real=9.9764171139512925e-001;zB[6].imag=-9.4997704948177541e-004;
  zB[7].real=9.9999655668583243e-001;zB[7].imag=2.9694976884679805e-005;
  zB[8].real=1.0000003470018055e+000;zB[8].imag=-5.3354333100097443e-008;
  zB[9].real=9.9999999831776032e-001;zB[9].imag=-3.7634408149196452e-009;

  vzTanh(vec_len,zA,zBha0);

  vmzTanh(vec_len,zA,zBep1,VML_EP);

  vmlSetMode(VML_EP);
  vzTanh(vec_len,zA,zBep2);

  vmzTanh(vec_len,zA,zBla1,VML_LA);

  vmlSetMode(VML_LA);
  vzTanh(vec_len,zA,zBla2);

  vmzTanh(vec_len,zA,zBha1,VML_HA);

  vmlSetMode(VML_HA);
  vzTanh(vec_len,zA,zBha2);

  for(i=0;i<10;i++) {
    if(zBha0[i].real!=zBha1[i].real || zBha0[i].imag!=zBha1[i].imag || zBha1[i].real!=zBha2[i].real || zBha1[i].imag!=zBha2[i].imag) {
      printf("Error! Difference between vzTanh and vmzTanh in VML_HA mode detected.\n");
      return 1;
    }

    if(zBla1[i].real!=zBla2[i].real || zBla1[i].imag!=zBla2[i].imag) {
      printf("Error! Difference between vzTanh and vmzTanh in VML_LA mode detected.\n");
      return 1;
    }

    if(zBep1[i].real!=zBep2[i].real || zBep1[i].imag!=zBep2[i].imag) {
      printf("Error! Difference between vzTanh and vmzTanh in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vzTanh test/example program\n\n");
  printf("           Argument                           vzTanh\n");
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
