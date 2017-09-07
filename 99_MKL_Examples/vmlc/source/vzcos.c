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
!    vzCos  Example Program Text
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
  zB[0].real=-9.2408901863462197e+003;zB[0].imag=-5.9914311829794678e+003;
  zB[1].real=9.0945453476834871e+001;zB[1].imag=1.1899188304247702e+003;
  zB[2].real=9.6572827892160802e+001;zB[2].imag=-8.6020414743525407e+001;
  zB[3].real=-1.3776099944555909e+001;zB[3].imag=-2.6670184875534431e+000;
  zB[4].real=7.4690514521306817e-001;zB[4].imag=1.2136622304193763e+000;
  zB[5].real=7.4690514521306817e-001;zB[5].imag=1.2136622304193763e+000;
  zB[6].real=-1.3776099944555909e+001;zB[6].imag=-2.6670184875534431e+000;
  zB[7].real=9.6572827892160802e+001;zB[7].imag=-8.6020414743525407e+001;
  zB[8].real=9.0945453476834871e+001;zB[8].imag=1.1899188304247702e+003;
  zB[9].real=-9.2408901863462197e+003;zB[9].imag=-5.9914311829794678e+003;

  vzCos(vec_len,zA,zBha0);

  vmzCos(vec_len,zA,zBep1,VML_EP);

  vmlSetMode(VML_EP);
  vzCos(vec_len,zA,zBep2);

  vmzCos(vec_len,zA,zBla1,VML_LA);

  vmlSetMode(VML_LA);
  vzCos(vec_len,zA,zBla2);

  vmzCos(vec_len,zA,zBha1,VML_HA);

  vmlSetMode(VML_HA);
  vzCos(vec_len,zA,zBha2);

  for(i=0;i<10;i++) {
    if(zBha0[i].real!=zBha1[i].real || zBha0[i].imag!=zBha1[i].imag || zBha1[i].real!=zBha2[i].real || zBha1[i].imag!=zBha2[i].imag) {
      printf("Error! Difference between vzCos and vmzCos in VML_HA mode detected.\n");
      return 1;
    }

    if(zBla1[i].real!=zBla2[i].real || zBla1[i].imag!=zBla2[i].imag) {
      printf("Error! Difference between vzCos and vmzCos in VML_LA mode detected.\n");
      return 1;
    }

    if(zBep1[i].real!=zBep2[i].real || zBep1[i].imag!=zBep2[i].imag) {
      printf("Error! Difference between vzCos and vmzCos in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vzCos test/example program\n\n");
  printf("           Argument                           vzCos\n");
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
