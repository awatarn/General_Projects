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
!    Calculation of quantiles and order statistics  Example Program Text
!******************************************************************************/

#include <stdio.h>

#include "mkl.h"
#include "errcheck.inc"
#include "generatedata.inc"

#define DIM     3       /* Task dimension */
#define N       1000    /* Number of observations */
#define M       9       /* Number of deciles */

int main()
{
    VSLSSTaskPtr task;
    MKL_INT dim;
    MKL_INT n;
    MKL_INT m;
    MKL_INT x_storage;
    MKL_INT o_storage;
    double x[DIM][N];       /* matrix of observations */
    double xT[N][DIM];      /* transposed matrix of observations */
    double o_stat[DIM][N];  /* matrix to store order statistics */
    double o_quant[M];
    double quantiles[DIM][M];
    double a = 0.0, sigma = 1.0;
    int i, j, errcode;
    int numRight = 0, numLeft = 0;
    int errnums = 0;

    /***** Initializing parameters for Summary Statistics task *****/
    dim              = DIM;
    n                = N;
    m                = M;
    x_storage        = VSL_SS_MATRIX_STORAGE_ROWS;
    o_storage        = VSL_SS_MATRIX_STORAGE_ROWS;

    /***** Generate transposed data set using VSL Gaussian RNG
    with mean a = 0 and stdev = 1 *****/
    errcode = dGenerateGaussianData( (double*)xT, dim, n, a, sigma );
    CheckVslError(errcode);

    for( j = 0; j < dim; j++ )
    {
        for( i = 0; i < n; i++ )
        {
            x[j][i] = xT[i][j];
        }
    }

    for ( i = 0; i < m; i++ )
    {
        o_quant[i] = (double)(i + 1) / (double)(m + 1);
    }

    /***** Create Summary Statistics task *****/
    errcode = vsldSSNewTask( &task, &dim, &n, &x_storage, (double*)x, 0, 0 );
    CheckVslError(errcode);

    /***** Edit task parameters for deciles computation *****/
    errcode = vsldSSEditQuantiles( task, &m, o_quant,
                                   (double*)quantiles, (double*)o_stat,
                                   &o_storage );
    CheckVslError(errcode);

    /***** Compute quantiles and order statistics using FAST method *****/
    errcode = vsldSSCompute( task, VSL_SS_QUANTS|VSL_SS_ORDER_STATS,
                             VSL_SS_METHOD_FAST );
    CheckVslError(errcode);

    /***** Check the correctness of computed quantiles and order
           statistics *****/
    for( j = 0; j < dim; j++ )
    {
        for ( i = 0; i < n - 1; i++ )
        {
            if ( o_stat[j][i] > o_stat[j][i + 1] ) errnums++;
        }

        numRight = numLeft = 0;

        for ( i = 0; i < n; i++ )
        {
            if( x[j][i] >= quantiles[j][m / 2] ) numRight++;
            if( x[j][i] <= quantiles[j][m / 2] ) numLeft++;
        }

        if ( numRight < (n/2 + (n & 1)) || numLeft < (n/2 + (n & 1)) ) errnums++;
    }

    /***** Printing results *****/
    printf("Task dimension : %d\n", dim);
    printf("Number of observations : %d\n\n", n);

    /***** Printing part of the initial matrix of observations *****/
    printf("\n1st 4 and last 4 observations in source matrix\n");
    for ( j = 0; j < dim; j++ )
    {
        for ( i = 0; i < 4; i++ )
        {
            printf("%+.3f ", x[j][i] );
        }
        printf("     ...      ");
        for ( i = n - 5; i < n; i++ )
        {
            printf("%+.3f ", x[j][i] );
        }
        printf("\n");
    }

    /***** Printing computed quantiles *****/
    printf("\nDeciles of the observations for all variables:\n   ");
    for ( i = 0; i < m; i++ ) printf("D%i     ", i + 1 );
    printf("\n");
    for ( j = 0; j < dim; j++ )
    {
        for ( i = 0; i < m; i++ ) printf("%+.3f ", quantiles[j][i] );
        printf("\n");
    }

    /***** Printing part of the order statistics matrix *****/
    printf("\n1st 4 and last 4 observations in order statistics matrix\n");
    for ( j = 0; j < dim; j++ )
    {
        for ( i = 0; i < 4; i++ )
        {
            printf("%+.3f ", o_stat[j][i] );
        }
        printf("     ...      ");
        for ( i = n - 5; i < n; i++ )
        {
            printf("%+.3f ", o_stat[j][i] );
        }
        printf("\n");
    }

    /***** Printing summary of the test *****/
    if (errnums == 0) {
        printf("\n\nComputed quantiles and order statistics");
        printf(" agree with theory.\n");
    }
    else {
        printf("\n\nError: Computed quantiles and/or order statistics");
        printf(" disagree with theory\n");
        return 1;
    }

    /***** Delete Summary Statistics task *****/
    errcode = vslSSDeleteTask( &task );
    CheckVslError(errcode);

    MKL_Free_Buffers();

    return 0;
}