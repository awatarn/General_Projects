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
!    vzCIS  Example Program Text
!******************************************************************************/

#include <stdio.h>
#include "mkl_vml.h"

#include "_rms.h"

int main()
{
  double dA[10];
  MKL_Complex16 zB[10];
  MKL_Complex16 zBha0[10],zBha1[10],zBha2[10];
  MKL_Complex16           zBla1[10],zBla2[10];
  MKL_Complex16           zBep1[10],zBep2[10];
  float CurRMS,MaxRMS=0.0;

  MKL_INT i=0,vec_len=10;

  dA[0]=-10000.0000;
  dA[1]=-7777.7777;
  dA[2]=-5555.5555;
  dA[3]=-3333.3333;
  dA[4]=-1111.1111;
  dA[5]=1111.1111;
  dA[6]=3333.3333;
  dA[7]=5555.5555;
  dA[8]=7777.7777;
  dA[9]=10000.0000;
  zB[0].real=-9.5215536825901481e-001;zB[0].imag=3.0561438888825215e-001;
  zB[1].real=6.9259906263181004e-001;zB[1].imag=7.2132276994528466e-001;
  zB[2].real=3.4393830299014322e-001;zB[2].imag=-9.3899224902885137e-001;
  zB[3].real=-9.9464921859866051e-001;zB[3].imag=1.0330988307550266e-001;
  zB[4].real=5.2957287328011915e-001;zB[4].imag=8.4826444690664649e-001;
  zB[5].real=5.2957287328011915e-001;zB[5].imag=-8.4826444690664649e-001;
  zB[6].real=-9.9464921859866051e-001;zB[6].imag=-1.0330988307550266e-001;
  zB[7].real=3.4393830299014322e-001;zB[7].imag=9.3899224902885137e-001;
  zB[8].real=6.9259906263181004e-001;zB[8].imag=-7.2132276994528466e-001;
  zB[9].real=-9.5215536825901481e-001;zB[9].imag=-3.0561438888825215e-001;

  vzCIS(vec_len,dA,zBha0);

  vmzCIS(vec_len,dA,zBep1,VML_EP);

  vmlSetMode(VML_EP);
  vzCIS(vec_len,dA,zBep2);

  vmzCIS(vec_len,dA,zBla1,VML_LA);

  vmlSetMode(VML_LA);
  vzCIS(vec_len,dA,zBla2);

  vmzCIS(vec_len,dA,zBha1,VML_HA);

  vmlSetMode(VML_HA);
  vzCIS(vec_len,dA,zBha2);

  for(i=0;i<10;i++) {
    if(zBha0[i].real!=zBha1[i].real || zBha0[i].imag!=zBha1[i].imag || zBha1[i].real!=zBha2[i].real || zBha1[i].imag!=zBha2[i].imag) {
      printf("Error! Difference between vzCIS and vmzCIS in VML_HA mode detected.\n");
      return 1;
    }

    if(zBla1[i].real!=zBla2[i].real || zBla1[i].imag!=zBla2[i].imag) {
      printf("Error! Difference between vzCIS and vmzCIS in VML_LA mode detected.\n");
      return 1;
    }

    if(zBep1[i].real!=zBep2[i].real || zBep1[i].imag!=zBep2[i].imag) {
      printf("Error! Difference between vzCIS and vmzCIS in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vzCIS test/example program\n\n");
  printf("           Argument                           vzCIS\n");
  printf("===============================================================================\n");
  for(i=0;i<10;i++) {
    printf("   % .4f      % .10f % +.10f*i\n",dA[i],zBha0[i].real,zBha0[i].imag);
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
