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
!    vmlSetErrorCallBack/vmlGetErrorCallBack/vmlClearErrorCallBack  Example
!    Program Text
!******************************************************************************/

#include <stdio.h>
#include "mkl_vml.h"

int UserCallBack(DefVmlErrorContext* );

int main()
{
  VMLErrorCallBack errcb;
  double dbA = 0.0;
  double dbR;

  printf("Set/Get/Clear CallBack example program\n\n");

  errcb=vmlGetErrorCallBack();
  printf("Initial adress of CallBack function: 0x%p\n",errcb);

  errcb=UserCallBack;
  vmlSetErrorCallBack(errcb);
  errcb=vmlGetErrorCallBack();
  printf("Adress of CallBack function after using Set CallBack: 0x%p\n",errcb);
  printf("Test user callback on vdLn function\n");
  vdLn(1, &dbA, &dbR);


  vmlClearErrorCallBack();
  errcb=vmlGetErrorCallBack();
  printf("Adress of CallBack function after using Clear CallBack: 0x%p\n",errcb);

  return 0;
}

int UserCallBack(DefVmlErrorContext* pdefVmlErrorContext)
{
    printf("In function %s argument a[%d]=%f is wrong.\n",
      pdefVmlErrorContext->cFuncName,
      pdefVmlErrorContext->iIndex,
      pdefVmlErrorContext->dbA1);
    return 0;
}
