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
!    vmlSetErrStatus/vmlGetErrStatus/vmlClearErrStatus example program  Example
!    Program Text
!******************************************************************************/

#include <stdio.h>
#include "mkl_vml.h"

void PrintTextVmlErrStatus(int );

int main()
{
  MKL_INT errst;

  printf("vmlSetErrStatus/vmlGetErrStatus/vmlClearErrStatus example program\n\n");

  errst=(MKL_INT)vmlGetErrStatus();

  printf("Initial value of vmlErrStatus: ");
  PrintTextVmlErrStatus((int)errst);
  printf(" (0x%x)\n",(int)errst);

  errst=(MKL_INT)(VML_STATUS_BADMEM);
  vmlSetErrStatus(errst);
  errst=(int)vmlGetErrStatus();
  printf("Value of vmlErrStatus after using vmlSetErrStatus: ");
  PrintTextVmlErrStatus((int)errst);
  printf(" (0x%x)\n",(int)errst);

  vmlClearErrStatus();
  errst=(MKL_INT)vmlGetErrStatus();
  printf("Value of vmlErrStatus after using vmlClearErrStatus: ");
  PrintTextVmlErrStatus((int)errst);
  printf(" (0x%x)\n",(int)errst);

  return 0;
}

void PrintTextVmlErrStatus(int errst)
{
  switch(errst) {
    case VML_STATUS_OK: {
                printf("VML_STATUS_OK");
                break;
    }
    case VML_STATUS_BADSIZE: {
                printf("VML_STATUS_BADSIZE");
                break;
    }
    case VML_STATUS_BADMEM: {
                printf("VML_STATUS_BADMEM");
                break;
    }
    case VML_STATUS_ERRDOM: {
                printf("VML_STATUS_ERRDOM");
                break;
    }
    case VML_STATUS_SING: {
                printf("VML_STATUS_SING");
                break;
    }
    case VML_STATUS_OVERFLOW: {
                printf("VML_STATUS_OVERFLOW");
                break;
    }
    case VML_STATUS_UNDERFLOW: {
                printf("VML_STATUS_UNDERFLOW");
                break;
    }
  }
}
