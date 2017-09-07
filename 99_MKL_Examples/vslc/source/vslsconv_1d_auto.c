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
!  Example of 1-dimension linear convolution operation on single precision data.
!*******************************************************************************/

#include <math.h>
#include <stdio.h>

#include "mkl_vsl.h"

int main()
{
    VSLConvTaskPtr task;
    float x[4]={1,2,3,4};
    float y[8]={11,12,13,14,15,16,17,18};
    float z[11]={0,0,0,0,0,0,0,0,0,0,0};
    float e[11]={11,34,70,120,130,140,150,160,151,122,72};
    MKL_INT xshape=4, yshape=8, zshape=11;
    int status,i;

    int mode = VSL_CONV_MODE_AUTO;

    /*
    *  Create task descriptor (create descriptor of problem)
    */
    status = vslsConvNewTask1D(&task,mode,xshape,yshape,zshape);
    if( status != VSL_STATUS_OK ){
        printf("ERROR: creation of job failed, exit with %d\n", status);
        return 1;
    }

    /*
    *  Execute task (Calculate 2 dimension convolution of two arrays)
    */
    status = vslsConvExec1D(task,x,1,y,1,z,1);
    if( status != VSL_STATUS_OK ){
        printf("ERROR: job status bad, exit with %d\n", status);
        return 1;
    }

    /*
    *  Delete task object (delete descriptor of problem)
    */
    status = vslConvDeleteTask(&task);
    if( status != VSL_STATUS_OK ){
        printf("ERROR: failed to delete task object, exit with %d\n", status);
        return 1;
    }

    /*
    * Check resulst for correctness:
    */

    for (i=0; i<zshape; i++)
        if (fabs(z[i]-e[i]) > fabs(e[i])*1e-5) {
            printf("ERROR: wrong results:\n");
            printf("    z[%2d]: %g\n",i,z[i]);
            printf(" expected: %g\n",e[i]);
            printf("EXAMPLE FAILED\n");
            return 1;
        }

    printf("EXAMPLE PASSED\n");
    return 0;
}
