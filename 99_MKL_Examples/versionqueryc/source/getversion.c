/*******************************************************************************
* Copyright 1999-2017 Intel Corporation All Rights Reserved.
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
!      MKLGetVersion example program to obtain an MKLVersion structure that
!      contains the version information.
!******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "mkl_service.h"

int main(void)
{
   MKLVersion ver;
   int len=198;
   char buf[198];

   MKL_Get_Version_String(buf, len);
   printf("\n%s\n",buf);
   printf("\n");

   MKL_Get_Version(&ver);
   printf("Major version:          %d\n",ver.MajorVersion);
   printf("Minor version:          %d\n",ver.MinorVersion);
   printf("Update version:         %d\n",ver.UpdateVersion);
   printf("Product status:         %s\n",ver.ProductStatus);
   printf("Build:                  %s\n",ver.Build);
   printf("Processor optimization: %s\n",ver.Processor);
   printf("================================================================\n");
   printf("\n");
   return 0;
}
