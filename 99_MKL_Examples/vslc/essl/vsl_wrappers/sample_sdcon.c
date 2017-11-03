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

#include "mkl_vsl.h"

#include <stdio.h>
#include <stdlib.h>

void sdcon(
    float h[], int inch,
    float x[], int incx,
    float y[], int incy,
    int nh, int nx, int iy0, int ny, int id)
{
    int status = VSL_STATUS_OK, error;
    VSLConvTaskPtr task, task_ptr=&task;

    vslsConvNewTask1D(task_ptr,VSL_CONV_MODE_DIRECT,nh,nx,ny);
    vslConvSetStart(task,&iy0);
    vslConvSetDecimation(task,&id);
    status = vslsConvExec1D(task,h,inch,x,incx,y,incy);

    error = vslConvDeleteTask(task_ptr);

    if (status != VSL_STATUS_OK) {
        printf("ERROR: sdcon(): bad status=%d\n",status);
        exit(1);
    }
    if (error != 0) {
        printf("ERROR: sdcon(): failed to destroy the task descriptor\n");
        exit(1);
    }
}