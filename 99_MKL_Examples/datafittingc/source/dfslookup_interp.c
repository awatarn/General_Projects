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
!    Interpolation with Look up method
!    Example Program Text
!******************************************************************************/

#include <stdio.h>

#include "mkl.h"
#include "errcheck.inc"
#include "generatedata.inc"
#include "rescheck.inc"

#define N              7 // number of break points
#define NY             1 // number of functions
#define NSITE          N // number of sites

#define NDORDER        1 // size of array describing derivative orders
                         // to compute
#define NDER     NDORDER // number of derivatives to compute
#define LEFT_LIMIT   1.0 // left limit of interpolation interval
#define RIGHT_LIMIT  3.0 // right limit of interpolation interval
#define FREQ         0.5


int main()
{
    DFTaskPtr task;                     // Data Fitting task descriptor
    MKL_INT nx;                         // number of break points
    MKL_INT xhint;                      // additional info about break points
    MKL_INT ny;                         // number of functions
    MKL_INT yhint;                      // additional info about function

    float x[N];                        // array of break points
    float y[NY*N];                     // function values

    MKL_INT nsite;                      // total number of interpolation sites
    MKL_INT sitehint;                   // additional info about interpolation
    float site[NSITE];                  // array of interpolation sites
    MKL_INT ndorder;                    // size of array describing derivative
                                        // orders
    MKL_INT dorder[] = { 1 };           // only value to calculate
                                        // will be computed

    MKL_INT rhint;                      // interpolation results storage format
    float r[NSITE];                     // spline evaluation results

    MKL_INT stype, sorder;

    float *datahint;                    // additional info about structure
                                        // of arrays x and y

    float left = LEFT_LIMIT, right = RIGHT_LIMIT;
    float freq = FREQ;

    int i, j, errcode = 0;
    int errnums = 0;

    /***** Initializing parameters for Data Fitting task *****/
    sorder = DF_PP_STD;
    stype  = DF_LOOKUP_INTERPOLANT;

    /***** Additional info about structure of arrays x and y *****/
    /* No additional info is provided */
    datahint = 0;

    /***** Parameters describing interpolation interval *****/
    nx          = N;
    xhint       = DF_NON_UNIFORM_PARTITION;

    /***** Parameters describing function *****/
    ny          = NY;
    yhint       = DF_NO_HINT;

    /***** Additional info about structure of arrays x and y *****/
    /* No additional info is provided */
    datahint = 0;

    /***** Parameters describing interpolation sites *****/
    nsite      = NSITE;
    sitehint   = DF_NON_UNIFORM_PARTITION;

    /**** Parameter describing size of array for derivative orders *****/
    ndorder    = NDORDER;
    /**** Parameter describing interpolation results storage *****/
    rhint      = DF_MATRIX_STORAGE_ROWS;

    /***** Generate array of uniformly distributed break points *****/
    errcode = sUniformRandSortedData( x, left, right, nx );
    CheckDfError(errcode);

    /***** Generate function y = sin(2 * Pi * freq * x) *****/
    errcode = sSinDataNotUniformGrid( y, x, FREQ, nx );
    CheckDfError(errcode);

    /***** Generate interpolation sites *****/
    for (i = 0; i < nx; i++)
    {
        site[i] = x[i];
    }

    /***** Create Data Fitting task *****/
    errcode = dfsNewTask1D( &task, nx, x, xhint, ny, y, yhint );
    CheckDfError(errcode);

    /***** Edit task parameters for look up interpolant *****/
    errcode = dfsEditPPSpline1D( task, sorder, stype, 0, 0, 0, 0, 0, 0 );
    CheckDfError(errcode);

    /***** Interpolate using lookup method *****/
    errcode = dfsInterpolate1D( task, DF_INTERP, DF_METHOD_PP,
                                    nsite, site, sitehint, ndorder,
                                    dorder, datahint, r, rhint, 0 );
    CheckDfError(errcode);

    /***** Delete Data Fitting task *****/
    errcode = dfDeleteTask( &task );
    CheckDfError(errcode);

    /***** Check interpolation results *****/
    for( j = 0; j < nx; j++ )
    {
        if ( DF_ABS( r[j] - y[j] ) > EPSILON_DOUBLE )
            errnums++;
    }

    /***** Print results *****/
    printf("Number of break points : %d\n", (int)nx);
    printf("Number of sites : %d\n", (int)nsite);

    /***** rint given function and computed results *****/
    printf("\n i  x(i)        y(i)       r(i)\n");
    for( j = 0; j < nx; j++ )
    {
        printf(" %+lf   %+lf   %+lf\n", x[j], y[j], r[j]);
    }

    /***** Print summary of the test *****/
    if (errnums != 0)
    {
        printf("\n\nError: Computed interpolation results");
        printf(" are incorrect\n");
        return 1;
    }
    else
    {
        printf("\n\nComputed interpolation results");
        printf(" are correct\n");
    }

    return 0;
}