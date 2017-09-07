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
!    vcPow  Example Program Text
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

  cA[0].real=0.1000;cA[0].imag=7.0000;
  cA[1].real=0.8666;cA[1].imag=6.2333;
  cA[2].real=1.6333;cA[2].imag=5.4666;
  cA[3].real=2.4000;cA[3].imag=4.6999;
  cA[4].real=3.1666;cA[4].imag=3.9333;
  cA[5].real=3.9333;cA[5].imag=3.1666;
  cA[6].real=4.7000;cA[6].imag=2.3999;
  cA[7].real=5.4666;cA[7].imag=1.6333;
  cA[8].real=6.2333;cA[8].imag=0.8666;
  cA[9].real=7.0000;cA[9].imag=0.0999;
  cB[0].real=7.9222171168020995e-006;cB[0].imag=2.1083585901275048e-005;
  cB[1].real=6.4512072154176666e-004;cB[1].imag=9.1692323654694654e-005;
  cB[2].real=9.0503621800775043e-003;cB[2].imag=-1.2801471360156119e-002;
  cB[3].real=-1.5959425975423699e-001;cB[3].imag=-2.6566230943845598e-001;
  cB[4].real=-4.8997204132252605e+000;cB[4].imag=1.1363607778135290e+000;
  cB[5].real=4.1042341669135736e+000;cB[5].imag=6.8100665478272219e+001;
  cB[6].real=7.9823020507184594e+002;cB[6].imag=-5.7795938272522804e+001;
  cB[7].real=-2.3514348248209863e+003;cB[7].imag=-8.1467661426596324e+003;
  cB[8].real=-6.5480032831151962e+004;cB[8].imag=5.3650159355771051e+004;
  cB[9].real=7.8757355465682549e+005;cB[9].imag=2.3871476140480436e+005;

  vcPow(vec_len,cA,cA,cBha0);

  vmcPow(vec_len,cA,cA,cBep1,VML_EP);

  vmlSetMode(VML_EP);
  vcPow(vec_len,cA,cA,cBep2);

  vmcPow(vec_len,cA,cA,cBla1,VML_LA);

  vmlSetMode(VML_LA);
  vcPow(vec_len,cA,cA,cBla2);

  vmcPow(vec_len,cA,cA,cBha1,VML_HA);

  vmlSetMode(VML_HA);
  vcPow(vec_len,cA,cA,cBha2);

  for(i=0;i<10;i++) {
    if(cBha0[i].real!=cBha1[i].real || cBha0[i].imag!=cBha1[i].imag || cBha1[i].real!=cBha2[i].real || cBha1[i].imag!=cBha2[i].imag) {
      printf("Error! Difference between vcPow and vmcPow in VML_HA mode detected.\n");
      return 1;
    }

    if(cBla1[i].real!=cBla2[i].real || cBla1[i].imag!=cBla2[i].imag) {
      printf("Error! Difference between vcPow and vmcPow in VML_LA mode detected.\n");
      return 1;
    }

    if(cBep1[i].real!=cBep2[i].real || cBep1[i].imag!=cBep2[i].imag) {
      printf("Error! Difference between vcPow and vmcPow in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vcPow test/example program\n\n");
  printf("           Arguments                           vcPow\n");
  printf("===============================================================================\n");
  for(i=0;i<10;i++) {
    printf("   % .2f %+.2f*i   % .2f %+.2f*i      % .5f % +.5f*i\n",cA[i].real,cA[i].imag,cA[i].real,cA[i].imag,cBha0[i].real,cBha0[i].imag);
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
