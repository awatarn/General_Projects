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
!    vcCIS  Example Program Text
!******************************************************************************/

#include <stdio.h>
#include "mkl_vml.h"

#include "_rms.h"

int main()
{
  float fA[10];
  MKL_Complex8 cB[10];
  MKL_Complex8 cBha0[10],cBha1[10],cBha2[10];
  MKL_Complex8           cBla1[10],cBla2[10];
  MKL_Complex8           cBep1[10],cBep2[10];
  float CurRMS,MaxRMS=0.0;

  MKL_INT i=0,vec_len=10;

  fA[0]=-10000.0000;
  fA[1]=-7777.7778;
  fA[2]=-5555.5557;
  fA[3]=-3333.3333;
  fA[4]=-1111.1111;
  fA[5]=1111.1111;
  fA[6]=3333.3333;
  fA[7]=5555.5557;
  fA[8]=7777.7778;
  fA[9]=10000.0000;
  cB[0].real=-9.5215536825901481e-001;cB[0].imag=3.0561438888825215e-001;
  cB[1].real=6.9269429374198155e-001;cB[1].imag=7.2123131893817349e-001;
  cB[2].real=3.4378424494653864e-001;cB[2].imag=-9.3904866376910323e-001;
  cB[3].real=-9.9465418116759940e-001;cB[3].imag=1.0326209316981849e-001;
  cB[4].real=5.2955928772685279e-001;cB[4].imag=8.4827292823844636e-001;
  cB[5].real=5.2955928772685279e-001;cB[5].imag=-8.4827292823844636e-001;
  cB[6].real=-9.9465418116759940e-001;cB[6].imag=-1.0326209316981849e-001;
  cB[7].real=3.4378424494653864e-001;cB[7].imag=9.3904866376910323e-001;
  cB[8].real=6.9269429374198155e-001;cB[8].imag=-7.2123131893817349e-001;
  cB[9].real=-9.5215536825901481e-001;cB[9].imag=-3.0561438888825215e-001;

  vcCIS(vec_len,fA,cBha0);

  vmcCIS(vec_len,fA,cBep1,VML_EP);

  vmlSetMode(VML_EP);
  vcCIS(vec_len,fA,cBep2);

  vmcCIS(vec_len,fA,cBla1,VML_LA);

  vmlSetMode(VML_LA);
  vcCIS(vec_len,fA,cBla2);

  vmcCIS(vec_len,fA,cBha1,VML_HA);

  vmlSetMode(VML_HA);
  vcCIS(vec_len,fA,cBha2);

  for(i=0;i<10;i++) {
    if(cBha0[i].real!=cBha1[i].real || cBha0[i].imag!=cBha1[i].imag || cBha1[i].real!=cBha2[i].real || cBha1[i].imag!=cBha2[i].imag) {
      printf("Error! Difference between vcCIS and vmcCIS in VML_HA mode detected.\n");
      return 1;
    }

    if(cBla1[i].real!=cBla2[i].real || cBla1[i].imag!=cBla2[i].imag) {
      printf("Error! Difference between vcCIS and vmcCIS in VML_LA mode detected.\n");
      return 1;
    }

    if(cBep1[i].real!=cBep2[i].real || cBep1[i].imag!=cBep2[i].imag) {
      printf("Error! Difference between vcCIS and vmcCIS in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vcCIS test/example program\n\n");
  printf("           Argument                           vcCIS\n");
  printf("===============================================================================\n");
  for(i=0;i<10;i++) {
    printf("   % .4f      % .10f % +.10f*i\n",fA[i],cBha0[i].real,cBha0[i].imag);
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
