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
!    Calculation of mean absolute deviation  Example Program Text
!******************************************************************************/

#include <stdio.h>

#include <math.h>
#include "mkl.h"
#include "errcheck.inc"
#include "generatedata.inc"

#define DIM     3       /* Task dimension */
#define N       1000    /* Number of observations */

int main()
{
    VSLSSTaskPtr task;
    MKL_INT dim;
    MKL_INT n;
    MKL_INT m;
    MKL_INT x_storage;

    float x[DIM][N];       /* matrix of observations */
    float mnad[DIM];
    float a = 0.0, sigma = 1.0;
    int i, errcode;
    int errnums = 0;
    float dn=(float)N;

    float tD, tQ, tD2, s, sig, sD, deltaD;

    /***** Initializing parameters for Summary Statistics task *****/
    dim              = DIM;
    n                = N;
    x_storage        = VSL_SS_MATRIX_STORAGE_ROWS;

    /***** Generate transposed data set using VSL Gaussian RNG
    with mean a = 0 and stdev = 1 *****/
    errcode = sGenerateGaussianData( (float*)x, dim, n, a, sigma );
    CheckVslError(errcode);

    /***** Create Summary Statistics task *****/
    errcode = vslsSSNewTask( &task, &dim, &n, &x_storage, (float*)x, 0, 0 );
    CheckVslError(errcode);

    /***** Provide array of mean absolute deviation *****/
    errcode = vslsSSEditTask( task, VSL_SS_ED_MNAD, mnad );
    CheckVslError(errcode);

    /***** Compute mean absolute deviation using FAST method *****/
    errcode = vslsSSCompute( task, VSL_SS_MNAD, VSL_SS_METHOD_FAST );
    CheckVslError(errcode);

    /***** Check the correctness of computed mean absolute deviations *****/
    /***** Testing relies on property that for Gaussian distribution
           standard deviation estimate ~= 1.2533 * mnad ******/
    tD=sigma*sigma;
    tQ=720.0*sigma*sigma*sigma*sigma;
    tD2=tD*tD;
    s=((tQ-tD2)/dn)-(2.0*(tQ-2*tD2)/(dn*dn))+((tQ-3*tD2)/(dn*dn*dn));

    for ( i = 0; i < dim; i++ )
    {
        sig = 1.2533 * mnad[i];
        sD = sig * sig;
        deltaD = fabs((tD-sD) / sqrt(s));
        if  ( deltaD > 3.0 ) errnums++;
    }

    /***** Printing results *****/
    printf("Task dimension : %d\n", dim);
    printf("Number of observations : %d\n\n", n);

    /***** Printing computed mean absolute deviations *****/
    printf("\nMean absolute deviation for all variables\n");
    for ( i = 0; i < dim; i++ ) printf("MnAD%i ", i + 1 );
    printf("\n");
    for ( i = 0; i < dim; i++ )
    {
        printf("%.3f  ", mnad[i] );
    }
    printf("\n");

    /***** Printing summary of the test *****/
    if (errnums == 0) {
        printf("\nComputed mean absolute deviations");
        printf(" agree with theory.\n");
    }
    else {
        printf("\nError: Computed mean absolute deviations");
        printf(" disagree with theory\n");
        return 1;
    }

    /***** Delete Summary Statistics task *****/
    errcode = vslSSDeleteTask( &task );
    CheckVslError(errcode);

    MKL_Free_Buffers();

    return 0;
}
