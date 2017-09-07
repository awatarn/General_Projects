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
!    Calculation of min/max estimates  Example Program Text
!******************************************************************************/

#include <stdio.h>

#include "mkl.h"
#include "errcheck.inc"
#include "generatedata.inc"

#define DIM     3      /* Task dimension */
#define N       1000   /* Number of observations */

int main()
{
    VSLSSTaskPtr task;
    MKL_INT dim;
    MKL_INT n;
    MKL_INT storage;
    double x[N][DIM];
    double min_est[DIM], max_est[DIM];
    double a = 0.0, sigma = 1.0;
    int i, j, errcode;
    int errnums = 0;

    /***** Initializing parameters for Summary Statistics task *****/
    dim     = DIM;
    n       = N;
    storage = VSL_SS_MATRIX_STORAGE_COLS;

    /***** Generate data set using VSL Gaussian RNG
           with mean a = 0 and stdev = 1 *****/
    errcode = dGenerateGaussianData( (double*)x, dim, n, a, sigma );
    CheckVslError(errcode);

    /***** Set initial values of the estimates *****/
    for(i = 0; i < dim; i++)
    {
        min_est[i] = max_est[i] = x[0][i];
    }

    /***** Create Summary Statistics task *****/
    errcode = vsldSSNewTask( &task, &dim, &n, &storage, (double*)x, 0, 0 );
    CheckVslError(errcode);

    /***** Edit task parameters for min and max computation *****/
    errcode = vsldSSEditTask( task, VSL_SS_ED_MIN, min_est );
    CheckVslError(errcode);

    errcode = vsldSSEditTask( task, VSL_SS_ED_MAX, max_est );
    CheckVslError(errcode);

    /***** Compute min and max estimates using FAST method *****/
    errcode = vsldSSCompute( task, VSL_SS_MIN | VSL_SS_MAX,
                             VSL_SS_METHOD_FAST );
    CheckVslError(errcode);

    /***** Comparison of observations with min and max estimates *****/
    for(i = 0; i < dim; i++)
    {
        for(j = 0; j < n; j++)
        {
            if(x[j][i] < min_est[i]) errnums++;
            if(x[j][i] > max_est[i]) errnums++;
        }
    }

    /***** Printing results *****/
    printf("Task dimension : %d\n", dim);
    printf("Number of observations : %d\n\n", n);

    printf("               Min        Max\n");
    for(i = 0; i < dim; i++)
    {
        printf("Variable #%i:  %+lf  %+lf\n", i + 1,
            min_est[i], max_est[i]);
    }

    if (errnums == 0) {
        printf("\n\nAll observations are within ranges for all dimensions\n");
    }
    else {
        printf("\n\nError: There are %i observations beyond the ranges\n", errnums);
        return 1;
    }

    /***** Delete Summary Statistics task *****/
    errcode = vslSSDeleteTask( &task );
    CheckVslError(errcode);

    MKL_Free_Buffers();

    return 0;
}
