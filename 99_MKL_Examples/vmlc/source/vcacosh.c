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
!    vcAcosh  Example Program Text
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

  cA[0].real=3.5000;cA[0].imag=10000.0000;
  cA[1].real=1114.2222;cA[1].imag=8889.2773;
  cA[2].real=2224.9443;cA[2].imag=7778.5557;
  cA[3].real=3335.6665;cA[3].imag=6667.8335;
  cA[4].real=4446.3887;cA[4].imag=5557.1113;
  cA[5].real=5557.1113;cA[5].imag=4446.3887;
  cA[6].real=6667.8335;cA[6].imag=3335.6665;
  cA[7].real=7778.5557;cA[7].imag=2224.9443;
  cA[8].real=8889.2773;cA[8].imag=1114.2222;
  cA[9].real=10000.0000;cA[9].imag=3.5000;
  cB[0].real=9.9034876162861227e+000;cB[0].imag=1.5704463720321655e+000;
  cB[1].real=9.7935427720062638e+000;cB[1].imag=1.4461021423339844e+000;
  cB[2].real=9.6915938771298862e+000;cB[2].imag=1.2921996116638184e+000;
  cB[3].real=9.6098742153837620e+000;cB[3].imag=1.1069388389587402e+000;
  cB[4].real=9.5633904224721853e+000;cB[4].imag=8.9597856998443604e-001;
  cB[5].real=9.5633904203067921e+000;cB[5].imag=6.7481774091720581e-001;
  cB[6].real=9.6098742099898065e+000;cB[6].imag=4.6385756134986877e-001;
  cB[7].real=9.6915938706466100e+000;cB[7].imag=2.7859675884246826e-001;
  cB[8].real=9.7935427659692937e+000;cB[8].imag=1.2469419836997986e-001;
  cB[9].real=9.9034876112861259e+000;cB[9].imag=3.4999998752027750e-004;

  vcAcosh(vec_len,cA,cBha0);

  vmcAcosh(vec_len,cA,cBep1,VML_EP);

  vmlSetMode(VML_EP);
  vcAcosh(vec_len,cA,cBep2);

  vmcAcosh(vec_len,cA,cBla1,VML_LA);

  vmlSetMode(VML_LA);
  vcAcosh(vec_len,cA,cBla2);

  vmcAcosh(vec_len,cA,cBha1,VML_HA);

  vmlSetMode(VML_HA);
  vcAcosh(vec_len,cA,cBha2);

  for(i=0;i<10;i++) {
    if(cBha0[i].real!=cBha1[i].real || cBha0[i].imag!=cBha1[i].imag || cBha1[i].real!=cBha2[i].real || cBha1[i].imag!=cBha2[i].imag) {
      printf("Error! Difference between vcAcosh and vmcAcosh in VML_HA mode detected.\n");
      return 1;
    }

    if(cBla1[i].real!=cBla2[i].real || cBla1[i].imag!=cBla2[i].imag) {
      printf("Error! Difference between vcAcosh and vmcAcosh in VML_LA mode detected.\n");
      return 1;
    }

    if(cBep1[i].real!=cBep2[i].real || cBep1[i].imag!=cBep2[i].imag) {
      printf("Error! Difference between vcAcosh and vmcAcosh in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vcAcosh test/example program\n\n");
  printf("           Argument                           vcAcosh\n");
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
