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
!    vcAcos  Example Program Text
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

  cA[0].real=-0.9000;cA[0].imag=0.9000;
  cA[1].real=-0.7000;cA[1].imag=0.7000;
  cA[2].real=-0.5000;cA[2].imag=0.5000;
  cA[3].real=-0.2999;cA[3].imag=0.2999;
  cA[4].real=-0.0999;cA[4].imag=0.0999;
  cA[5].real=0.0999;cA[5].imag=-0.0999;
  cA[6].real=0.3000;cA[6].imag=-0.3000;
  cA[7].real=0.5000;cA[7].imag=-0.5000;
  cA[8].real=0.7000;cA[8].imag=-0.7000;
  cA[9].real=0.9000;cA[9].imag=-0.9000;
  cB[0].real=2.2123245686712978e+000;cB[0].imag=-9.6595638990402222e-001;
  cB[1].real=2.1393931187077269e+000;cB[1].imag=-7.5646597146987915e-001;
  cB[2].real=2.0230747739460875e+000;cB[2].imag=-5.3063750267028809e-001;
  cB[3].real=1.8610635871160541e+000;cB[3].imag=-3.0809590220451355e-001;
  cB[4].real=1.6703610431709226e+000;cB[4].imag=-1.0022931545972824e-001;
  cB[5].real=1.4712316104188707e+000;cB[5].imag=1.0022931545972824e-001;
  cB[6].real=1.2804390572597750e+000;cB[6].imag=3.0820351839065552e-001;
  cB[7].real=1.1185178796437059e+000;cB[7].imag=5.3063750267028809e-001;
  cB[8].real=1.0021995348820663e+000;cB[8].imag=7.5646597146987915e-001;
  cB[9].real=9.2926808491849544e-001;cB[9].imag=9.6595638990402222e-001;

  vcAcos(vec_len,cA,cBha0);

  vmcAcos(vec_len,cA,cBep1,VML_EP);

  vmlSetMode(VML_EP);
  vcAcos(vec_len,cA,cBep2);

  vmcAcos(vec_len,cA,cBla1,VML_LA);

  vmlSetMode(VML_LA);
  vcAcos(vec_len,cA,cBla2);

  vmcAcos(vec_len,cA,cBha1,VML_HA);

  vmlSetMode(VML_HA);
  vcAcos(vec_len,cA,cBha2);

  for(i=0;i<10;i++) {
    if(cBha0[i].real!=cBha1[i].real || cBha0[i].imag!=cBha1[i].imag || cBha1[i].real!=cBha2[i].real || cBha1[i].imag!=cBha2[i].imag) {
      printf("Error! Difference between vcAcos and vmcAcos in VML_HA mode detected.\n");
      return 1;
    }

    if(cBla1[i].real!=cBla2[i].real || cBla1[i].imag!=cBla2[i].imag) {
      printf("Error! Difference between vcAcos and vmcAcos in VML_LA mode detected.\n");
      return 1;
    }

    if(cBep1[i].real!=cBep2[i].real || cBep1[i].imag!=cBep2[i].imag) {
      printf("Error! Difference between vcAcos and vmcAcos in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vcAcos test/example program\n\n");
  printf("           Argument                           vcAcos\n");
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
