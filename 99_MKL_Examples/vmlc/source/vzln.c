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
!    vzLn  Example Program Text
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

  zA[0].real=0.1000;zA[0].imag=10000.0000;
  zA[1].real=1111.1999;zA[1].imag=8888.9000;
  zA[2].real=2222.2999;zA[2].imag=7777.8000;
  zA[3].real=3333.3999;zA[3].imag=6666.7000;
  zA[4].real=4444.5000;zA[4].imag=5555.6000;
  zA[5].real=5555.6000;zA[5].imag=4444.5000;
  zA[6].real=6666.7000;zA[6].imag=3333.4000;
  zA[7].real=7777.8000;zA[7].imag=2222.3000;
  zA[8].real=8888.9000;zA[8].imag=1111.2000;
  zA[9].real=10000.0000;zA[9].imag=0.1000;
  zB[0].real=9.2103403720261827e+000;zB[0].imag=1.5707863267948969e+000;
  zB[1].real=9.1003118897854645e+000;zB[1].imag=1.4464316510411834e+000;
  zB[2].real=8.9982670310606725e+000;zB[2].imag=1.2924881891553042e+000;
  zB[3].real=8.9164550335110988e+000;zB[3].imag=1.1071427298419583e+000;
  zB[4].real=8.8699115840444964e+000;zB[4].imag=8.9605318947080836e-001;
  zB[5].real=8.8699115840444964e+000;zB[5].imag=6.7474313732408819e-001;
  zB[6].real=8.9164550395111224e+000;zB[6].imag=4.6365360895280644e-001;
  zB[7].real=8.9982670344569815e+000;zB[7].imag=2.7830814952629329e-001;
  zB[8].real=9.1003118911701844e+000;zB[8].imag=1.2436468683059552e-001;
  zB[9].real=9.2103403720261827e+000;zB[9].imag=9.9999999996666679e-006;

  vzLn(vec_len,zA,zBha0);

  vmzLn(vec_len,zA,zBep1,VML_EP);

  vmlSetMode(VML_EP);
  vzLn(vec_len,zA,zBep2);

  vmzLn(vec_len,zA,zBla1,VML_LA);

  vmlSetMode(VML_LA);
  vzLn(vec_len,zA,zBla2);

  vmzLn(vec_len,zA,zBha1,VML_HA);

  vmlSetMode(VML_HA);
  vzLn(vec_len,zA,zBha2);

  for(i=0;i<10;i++) {
    if(zBha0[i].real!=zBha1[i].real || zBha0[i].imag!=zBha1[i].imag || zBha1[i].real!=zBha2[i].real || zBha1[i].imag!=zBha2[i].imag) {
      printf("Error! Difference between vzLn and vmzLn in VML_HA mode detected.\n");
      return 1;
    }

    if(zBla1[i].real!=zBla2[i].real || zBla1[i].imag!=zBla2[i].imag) {
      printf("Error! Difference between vzLn and vmzLn in VML_LA mode detected.\n");
      return 1;
    }

    if(zBep1[i].real!=zBep2[i].real || zBep1[i].imag!=zBep2[i].imag) {
      printf("Error! Difference between vzLn and vmzLn in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vzLn test/example program\n\n");
  printf("           Argument                           vzLn\n");
  printf("===============================================================================\n");
  for(i=0;i<10;i++) {
    printf("   % .4f %+.4f*i      % .10f % +.10f*i\n",zA[i].real,zA[i].imag,zBha0[i].real,zBha0[i].imag);
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
