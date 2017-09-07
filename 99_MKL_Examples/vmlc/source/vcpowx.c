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
!    vcPowx  Example Program Text
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
  cB[0].real=1.4660081731727082e+001;cB[0].imag=-4.2457664528605692e+000;
  cB[1].real=6.6546281414811581e+000;cB[1].imag=-1.3285049956433053e+001;
  cB[2].real=-7.0387482404509374e+000;cB[2].imag=-1.4767324238678261e+001;
  cB[3].real=-2.1117849271454592e+001;cB[3].imag=-3.4930927491228259e+000;
  cB[4].real=-2.4424384995282274e+001;cB[4].imag=2.4394003894154913e+001;
  cB[5].real=4.1042341669135736e+000;cB[5].imag=6.8100665478272219e+001;
  cB[6].real=1.0377464969389693e+002;cB[6].imag=1.1602925928663136e+002;
  cB[7].real=3.5030956185511837e+002;cB[7].imag=1.3713892387072394e+002;
  cB[8].real=8.9261612350130133e+002;cB[8].imag=7.6109263759848176e+001;
  cB[9].real=2.0121482556255414e+003;cB[9].imag=-1.3059791392822208e+002;

  vcPowx(vec_len,cA,cA[5],cBha0);

  vmcPowx(vec_len,cA,cA[5],cBep1,VML_EP);

  vmlSetMode(VML_EP);
  vcPowx(vec_len,cA,cA[5],cBep2);

  vmcPowx(vec_len,cA,cA[5],cBla1,VML_LA);

  vmlSetMode(VML_LA);
  vcPowx(vec_len,cA,cA[5],cBla2);

  vmcPowx(vec_len,cA,cA[5],cBha1,VML_HA);

  vmlSetMode(VML_HA);
  vcPowx(vec_len,cA,cA[5],cBha2);

  for(i=0;i<10;i++) {
    if(cBha0[i].real!=cBha1[i].real || cBha0[i].imag!=cBha1[i].imag || cBha1[i].real!=cBha2[i].real || cBha1[i].imag!=cBha2[i].imag) {
      printf("Error! Difference between vcPowx and vmcPowx in VML_HA mode detected.\n");
      return 1;
    }

    if(cBla1[i].real!=cBla2[i].real || cBla1[i].imag!=cBla2[i].imag) {
      printf("Error! Difference between vcPowx and vmcPowx in VML_LA mode detected.\n");
      return 1;
    }

    if(cBep1[i].real!=cBep2[i].real || cBep1[i].imag!=cBep2[i].imag) {
      printf("Error! Difference between vcPowx and vmcPowx in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vcPowx test/example program\n\n");
  printf("           Argument                           vcPowx\n");
  printf("===============================================================================\n");
  for(i=0;i<10;i++) {
    printf("   % .2f %+.2f*i   % .2f %+.2f*i      % .5f % +.5f*i\n",cA[i].real,cA[i].imag,cA[5].real,cA[5].imag,cBha0[i].real,cBha0[i].imag);
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
