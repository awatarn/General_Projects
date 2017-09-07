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
!    vcAtanh  Example Program Text
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

  cA[0].real=0.7500;cA[0].imag=0.9000;
  cA[1].real=0.7666;cA[1].imag=0.8833;
  cA[2].real=0.7833;cA[2].imag=0.8666;
  cA[3].real=0.8000;cA[3].imag=0.8500;
  cA[4].real=0.8166;cA[4].imag=0.8333;
  cA[5].real=0.8333;cA[5].imag=0.8166;
  cA[6].real=0.8500;cA[6].imag=0.8000;
  cA[7].real=0.8666;cA[7].imag=0.7833;
  cA[8].real=0.8833;cA[8].imag=0.7666;
  cA[9].real=0.9000;cA[9].imag=0.7500;
  cB[0].real=3.7257323907975143e-001;cB[0].imag=8.8743013143539429e-001;
  cB[1].real=3.8548667482649812e-001;cB[1].imag=8.8805592060089111e-001;
  cB[2].real=3.9865922844057755e-001;cB[2].imag=8.8905519247055054e-001;
  cB[3].real=4.1200697096525207e-001;cB[3].imag=8.9044255018234253e-001;
  cB[4].real=4.2558004817630984e-001;cB[4].imag=8.9211910963058472e-001;
  cB[5].real=4.3940256872758254e-001;cB[5].imag=8.9423400163650513e-001;
  cB[6].real=4.5338331929830056e-001;cB[6].imag=8.9679872989654541e-001;
  cB[7].real=4.6758174531596247e-001;cB[7].imag=8.9971578121185303e-001;
  cB[8].real=4.8200889281307913e-001;cB[8].imag=9.0314799547195435e-001;
  cB[9].real=4.9656447251629632e-001;cB[9].imag=9.0710288286209106e-001;

  vcAtanh(vec_len,cA,cBha0);

  vmcAtanh(vec_len,cA,cBep1,VML_EP);

  vmlSetMode(VML_EP);
  vcAtanh(vec_len,cA,cBep2);

  vmcAtanh(vec_len,cA,cBla1,VML_LA);

  vmlSetMode(VML_LA);
  vcAtanh(vec_len,cA,cBla2);

  vmcAtanh(vec_len,cA,cBha1,VML_HA);

  vmlSetMode(VML_HA);
  vcAtanh(vec_len,cA,cBha2);

  for(i=0;i<10;i++) {
    if(cBha0[i].real!=cBha1[i].real || cBha0[i].imag!=cBha1[i].imag || cBha1[i].real!=cBha2[i].real || cBha1[i].imag!=cBha2[i].imag) {
      printf("Error! Difference between vcAtanh and vmcAtanh in VML_HA mode detected.\n");
      return 1;
    }

    if(cBla1[i].real!=cBla2[i].real || cBla1[i].imag!=cBla2[i].imag) {
      printf("Error! Difference between vcAtanh and vmcAtanh in VML_LA mode detected.\n");
      return 1;
    }

    if(cBep1[i].real!=cBep2[i].real || cBep1[i].imag!=cBep2[i].imag) {
      printf("Error! Difference between vcAtanh and vmcAtanh in VML_EP mode detected.\n");
      return 1;
    }
  }

  printf("vcAtanh test/example program\n\n");
  printf("           Argument                           vcAtanh\n");
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
