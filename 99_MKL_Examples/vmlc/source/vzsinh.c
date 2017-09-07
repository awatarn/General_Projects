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
!    vzSinh  Example Program Text
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

  zA[0].real=-7.0000;zA[0].imag=7.0000;
  zA[1].real=-5.4444;zA[1].imag=5.4444;
  zA[2].real=-3.8888;zA[2].imag=3.8888;
  zA[3].real=-2.3333;zA[3].imag=2.3333;
  zA[4].real=-0.7777;zA[4].imag=0.7777;
  zA[5].real=0.7777;zA[5].imag=-0.7777;
  zA[6].real=2.3333;zA[6].imag=-2.3333;
  zA[7].real=3.8888;zA[7].imag=-3.8888;
  zA[8].real=5.4444;zA[8].imag=-5.4444;
  zA[9].real=7.0000;zA[9].imag=-7.0000;
  zB[0].real=-4.1337676142848187e+002;zB[0].imag=3.6023694394619349e+002;
  zB[1].real=-7.7348104204023088e+001;zB[1].imag=-8.6084653219059518e+001;
  zB[2].real=1.7911235047375367e+001;zB[2].imag=-1.6606765833296961e+001;
  zB[3].real=3.5279032191119786e+000;zB[3].imag=3.7633805209492461e+000;
  zB[4].real=-6.1170599172912610e-001;zB[4].imag=9.2473746870188400e-001;
  zB[5].real=6.1170599172912610e-001;zB[5].imag=-9.2473746870188400e-001;
  zB[6].real=-3.5279032191119786e+000;zB[6].imag=-3.7633805209492461e+000;
  zB[7].real=-1.7911235047375367e+001;zB[7].imag=1.6606765833296961e+001;
  zB[8].real=7.7348104204023088e+001;zB[8].imag=8.6084653219059518e+001;
  zB[9].real=4.1337676142848187e+002;zB[9].imag=-3.6023694394619349e+002;

  vzSinh(vec_len,zA,zBha0);

  vmzSinh(vec_len,zA,zBep1,VML_EP);

  vmlSetMode(VML_EP);
  vzSinh(vec_len,zA,zBep2);

  vmzSinh(vec_len,zA,zBla1,VML_LA);

  vmlSetMode(VML_LA);
  vzSinh(vec_len,zA,zBla2);

  vmzSinh(vec_len,zA,zBha1,VML_HA);

  vmlSetMode(VML_HA);
  vzSinh(vec_len,zA,zBha2);

  for(i=0;i<10;i++) {
    if(zBha0[i].real!=zBha1[i].real || zBha0[i].imag!=zBha1[i].imag || zBha1[i].real!=zBha2[i].real || zBha1[i].imag!=zBha2[i].imag) {
      printf("Error! Difference between vzSinh and vmzSinh in VML_HA mode detected.\n");
      return 1;
    }

    if(zBla1[i].real!=zBla2[i].real || zBla1[i].imag!=zBla2[i].imag) {
      printf("Error! Difference between vzSinh and vmzSinh in VML_LA mode detected.\n");
      return 1;
    }

    if(zBep1[i].real!=zBep2[i].real || zBep1[i].imag!=zBep2[i].imag) {
      printf("Error! Difference between vzSinh and vmzSinh in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vzSinh test/example program\n\n");
  printf("           Argument                           vzSinh\n");
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
