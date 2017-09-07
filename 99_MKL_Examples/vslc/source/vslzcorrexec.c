/*******************************************************************************
* Copyright 2003-2017 Intel Corporation All Rights Reserved.
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
!    vslzCorrExec  Example Program Text
!******************************************************************************/

#include "mkl_vsl.h"

#include <stdio.h>

#define XSHAPE 100
#define YSHAPE 1000
#define ZSHAPE (XSHAPE-1)+(YSHAPE-1)+1

int main()
{
    VSLCorrTaskPtr task;
    MKL_INT rank,mode,xshape,yshape,zshape;
    static MKL_Complex16 x[XSHAPE],y[YSHAPE],z[ZSHAPE];
    MKL_INT xstride=1,ystride=1,zstride=1;
    int status,ok,i;

    xshape=XSHAPE;
    yshape=YSHAPE;
    zshape=ZSHAPE;

    for (i=0; i<xshape; i++)
    {
        x[i].real = 0;
        x[i].imag = 0;
    }
    for (i=0; i<yshape; i++)
    {
        y[i].real = 0;
        y[i].imag = 0;
    }

    ok = 1;
    printf("EXAMPLE executing a correlation task\n");

    rank = 1;
    mode = VSL_CONV_MODE_AUTO;
    vslzCorrNewTask(&task,mode,rank,&xshape,&yshape,&zshape);

    status = vslzCorrExec(task,x,&xstride,y,&ystride,z,&zstride);

    if (status != VSL_STATUS_OK) {
        printf("ERROR: bad status: %d\n",status);
        ok = 0;
    }

    for (i=0; i<zshape; i++)
        if ((z[i].real != 0) || (z[i].imag != 0)) {
            printf("ERROR: wrong result: z[%d]=%lg + I*%lg\n",i,z[i].real,z[i].imag);
            ok = 0;
        }

    printf("EXAMPLE %s\n", ok? "PASSED": "FAILED");
    return !ok;
}
