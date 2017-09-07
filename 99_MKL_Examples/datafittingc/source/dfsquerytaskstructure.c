/*******************************************************************************
* Copyright 2010-2017 Intel Corporation All Rights Reserved.
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
!   Querying Data Fitting task parameters Example Program Text
!******************************************************************************/

#include <stdio.h>

#include "mkl.h"
#include "errcheck.inc"
#include "generatedata.inc"
#include "rescheck.inc"

#define N                     10    // number of breakpoints
#define NY                     2    // number of datasets to interpolate

int main()
{
    DFTaskPtr task;                 // Data Fitting task descriptor
    MKL_INT nx;                     // number of break points
    MKL_INT xhint;                  // additional info about break points
    MKL_INT ny;                     // number of functions
    MKL_INT yhint;                  // functions storage format

    float x[] = {-1.0f, 1.0f};      // break points
    float y[N*NY];                  // function values

    MKL_INT nx_ret;                 // parameters to be queried
    float *y_ret;
    float *y1_ret;

    int errnums, errcode = 0;

    /***** Initializing parameters for Data Fitting task *****/

    /***** Parameters describing interpolation interval *****/
    nx        = N;
    xhint     = DF_UNIFORM_PARTITION;

    /***** Parameters describing functions *****/
    ny         = NY;
    yhint      = DF_MATRIX_STORAGE_ROWS;

    /***** Create Data Fitting task *****/
    errcode = dfsNewTask1D( &task, nx, x, xhint, ny, y, yhint );
    CheckDfError(errcode);

    /***** Query Data Fitting task parameters *****/

    /***** Query value *****/
    errcode = dfiQueryVal( task, DF_NX, &nx_ret );
    CheckDfError(errcode);

    /***** Query pointer *****/
    errcode = dfsQueryPtr( task, DF_Y, &y_ret );
    CheckDfError(errcode);

    /***** Query pointer by index *****/
    errcode = dfsQueryIdxPtr( task, DF_Y, 1, &y1_ret );
    CheckDfError(errcode);

    /***** Check requested parameters *****/
    errnums = 0;
    if ( nx    != nx_ret ) errnums++;
    if ( y     != y_ret )  errnums++;
    if ( y + N != y1_ret ) errnums++;

    /***** Print results *****/
    printf("                                          Expected");
    printf("              Obtained\n");
    printf("Number of break points          : %16d      %16d\n",
        (int)nx, (int)nx_ret);
    printf("Address of function Y           : %16p      %16p\n", y, y_ret);
    printf("Address of 1-th coordinate of Y : %16p      %16p\n", y+N, y1_ret);

    /***** Delete Data Fitting task *****/
    errcode = dfDeleteTask( &task );
    CheckDfError(errcode);

    /***** Print summary of the test *****/
    if (errnums != 0)
    {
        printf("\n\nError: Not all requested parameters are correct\n");
        return 1;
    }
    else
    {
        printf("\n\nAll requested parameters are correct\n");
    }

    return 0;
}
