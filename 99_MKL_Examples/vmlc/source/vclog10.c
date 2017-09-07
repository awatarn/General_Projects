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
!    vcLog10  Example Program Text
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

  cA[0].real=0.1000;cA[0].imag=10000.0000;
  cA[1].real=1111.2000;cA[1].imag=8888.9004;
  cA[2].real=2222.2998;cA[2].imag=7777.7998;
  cA[3].real=3333.3999;cA[3].imag=6666.7002;
  cA[4].real=4444.5000;cA[4].imag=5555.6001;
  cA[5].real=5555.6001;cA[5].imag=4444.5000;
  cA[6].real=6666.7002;cA[6].imag=3333.3999;
  cA[7].real=7777.7998;cA[7].imag=2222.3000;
  cA[8].real=8888.9004;cA[8].imag=1111.2000;
  cA[9].real=10000.0000;cA[9].imag=0.1000;
  cB[0].real=4.0000000000217151e+000;cB[0].imag=6.8218386173248291e-001;
  cB[1].real=3.9522152564316348e+000;cB[1].imag=6.2817728519439697e-001;
  cB[2].real=3.9078977067930758e+000;cB[2].imag=5.6132048368453979e-001;
  cB[3].real=3.8723672294320828e+000;cB[3].imag=4.8082599043846130e-001;
  cB[4].real=3.8521536605751314e+000;cB[4].imag=3.8915094733238220e-001;
  cB[5].real=3.8521536605751314e+000;cB[5].imag=2.9303723573684692e-001;
  cB[6].real=3.8723672294320828e+000;cB[6].imag=2.0136220753192902e-001;
  cB[7].real=3.9078977103941459e+000;cB[7].imag=1.2086769193410873e-001;
  cB[8].real=3.9522152564316348e+000;cB[8].imag=5.4010897874832153e-002;
  cB[9].real=4.0000000000217151e+000;cB[9].imag=4.3429449760878924e-006;

  vcLog10(vec_len,cA,cBha0);

  vmcLog10(vec_len,cA,cBep1,VML_EP);

  vmlSetMode(VML_EP);
  vcLog10(vec_len,cA,cBep2);

  vmcLog10(vec_len,cA,cBla1,VML_LA);

  vmlSetMode(VML_LA);
  vcLog10(vec_len,cA,cBla2);

  vmcLog10(vec_len,cA,cBha1,VML_HA);

  vmlSetMode(VML_HA);
  vcLog10(vec_len,cA,cBha2);

  for(i=0;i<10;i++) {
    if(cBha0[i].real!=cBha1[i].real || cBha0[i].imag!=cBha1[i].imag || cBha1[i].real!=cBha2[i].real || cBha1[i].imag!=cBha2[i].imag) {
      printf("Error! Difference between vcLog10 and vmcLog10 in VML_HA mode detected.\n");
      return 1;
    }

    if(cBla1[i].real!=cBla2[i].real || cBla1[i].imag!=cBla2[i].imag) {
      printf("Error! Difference between vcLog10 and vmcLog10 in VML_LA mode detected.\n");
      return 1;
    }

    if(cBep1[i].real!=cBep2[i].real || cBep1[i].imag!=cBep2[i].imag) {
      printf("Error! Difference between vcLog10 and vmcLog10 in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vcLog10 test/example program\n\n");
  printf("           Argument                           vcLog10\n");
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
