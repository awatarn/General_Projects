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
!    Calculation of cross-product matrix Example Program Text
!******************************************************************************/

#include <stdio.h>

#include "mkl.h"
#include "errcheck.inc"
#include "generatedata.inc"
#include "statchars.inc"

#define DIM     4        /* Task dimension */
#define N       1000     /* Number of observations */

#define P_THRESHOLD     0.01

float C[DIM][DIM] = {
    { 1.0, 0.0, 0.0, 0.0 },
    { 0.0, 1.0, 0.0, 0.0 },
    { 0.0, 0.0, 1.0, 0.0 },
    { 0.0, 0.0, 0.0, 1.0 }
};
float a[DIM] = { 5.0, 5.0, 5.0, 5.0 };

int main()
{
    VSLSSTaskPtr task;
    MKL_INT dim;
    MKL_INT n;
    MKL_INT x_storage;
    MKL_INT cov_storage, cp_storage;

    float x[N][DIM];
    float mean[DIM], cp[DIM][DIM], cov[DIM][DIM];
    int i, j, errcode;
    unsigned MKL_INT64 estimate = 0;
    int errnums = 0;

    float pval_cov[DIM][DIM];

    /***** Initializing parameters for Summary Statistics task *****/
    dim         = DIM;
    n           = N;
    x_storage   = VSL_SS_MATRIX_STORAGE_COLS;
    cov_storage = VSL_SS_MATRIX_STORAGE_FULL;
    cp_storage  = VSL_SS_MATRIX_STORAGE_FULL;

    /***** Generate data set using VSL GaussianMV RNG *****/
    errcode = sGenerateGaussianMVData( (float*)x, dim, n, a, (float*)C );
    CheckVslError(errcode);

    /***** Create Summary Statistics task *****/
    errcode = vslsSSNewTask( &task, &dim, &n, &x_storage, (float*)x, 0, 0 );
    CheckVslError(errcode);

    /***** Initialization of the task parameters related
           to cross-product matrix computation *****/
    vslsSSEditCP(task, mean, 0, (float*)cp, &cp_storage);
    CheckVslError(errcode);

    /***** Cross-product matrix is included in the list of estimates
           to compute *****/
    estimate = VSL_SS_CP;

    /***** Compute the estimates using FAST method *****/
    errcode = vslsSSCompute( task, estimate, VSL_SS_METHOD_FAST );
    CheckVslError(errcode);


    /***** Edit task parameters for computation of covariance *****/
    errcode = vslsSSEditTask( task, VSL_SS_ED_COV, (float*)cov );
    CheckVslError(errcode);
    errcode = vsliSSEditTask( task, VSL_SS_ED_COV_STORAGE, &cov_storage );
    CheckVslError(errcode);

    /***** Convert cross-product matrix into correlation matrix *****/
    errcode = vslsSSCompute( task, VSL_SS_COV, VSL_SS_METHOD_CP_TO_COVCOR );
    CheckVslError(errcode);

    /***** Testing stat characteristics of the computed estimates *****/
    /* Compute p-values for covariance computed from cross-product */
    sComputePvalsVariance( dim, n, (float*)cov, cov_storage, (float*)C,
                           (float*)pval_cov );
    sComputePvalsCovariance( dim, n, (float*)cov, cov_storage, (float*)C,
                             (float*)pval_cov );

    /***** Checking the validity of p-values for all estimates *****/
    for(i = 0; i < dim; i++)
    {
        if (pval_cov[i][i] < P_THRESHOLD) errnums++;
        for(j = 0; j < i; j++)
        {
            if (pval_cov[i][j] < P_THRESHOLD) errnums++;
        }
    }

    /***** Printing results *****/
    printf("Task dimension : %d\n", dim);
    printf("Number of observations : %d\n\n", n);

    /***** Printing computed cross-product matrix *****/
    printf("\n Computed cross-product matrix\n");
    for(i = 0; i < dim; i++)
    {
        for(j = 0; j < dim; j++)
        {
            printf("%+9f ", cp[i][j]);
        }
        printf("\n");
    }

    /***** Printing p-values for covariance matrix estimate *****/
    printf("\n P-values for covariance obtained from cross-product matrix\n");
    for(i = 0; i < dim; i++)
    {
        for(j = 0; j < dim; j++)
        {
            printf("%+9f ", pval_cov[i][j]);
        }
        printf("\n");
    }

    /***** Printing summary of the test *****/
    if (errnums == 0)
    {
        printf("\n\nAll the computed estimates agree with theory\n");
    }
    else
    {
        printf("\n\nError: At least one of the computed estimates");
        printf(" disagrees with theory\n");
        return 1;
    }

    errcode = vslSSDeleteTask( &task );
    CheckVslError(errcode);

    MKL_Free_Buffers();

    return errcode;
}
