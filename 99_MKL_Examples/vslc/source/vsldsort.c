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
!    Sorting data array  Example Program Text
!******************************************************************************/

#include <stdio.h>

#include "mkl.h"
#include "mkl_vsl.h"
#include "errcheck.inc"
#include "generatedata.inc"

#define P       3    /* Number of observations */
#define N       10   /* Task dimension */
#define SEED    1    /* Initial value for stream initialization */

int main()
{
    VSLSSTaskPtr task;
    MKL_INT n;
    MKL_INT p;
    MKL_INT x_storage;
    MKL_INT sorted_x_storage;
    double x[P][N];                    /* Matrix of observations */
    double y[P][N];                    /* Output  matrix for sorted data */
    double lBound = 0.0, rBound = 10;  /* Bounds for uniform generator */
    int errnums = 0;
    int i, j, errcode;
    VSLStreamStatePtr stream;

    /***** Initializing parameters for Summary Statistics task *****/
    n = N;
    p = P;
    x_storage        = VSL_SS_MATRIX_STORAGE_ROWS;
    sorted_x_storage = VSL_SS_MATRIX_STORAGE_ROWS;

    /***** Generate data set using VSL Uniform RNG *****/
    /***** Initialize *****/
    errcode = vslNewStream(&stream, VSL_BRNG_MCG31, SEED);
    CheckVslError(errcode);

    /***** Call RNG *****/
    errcode = vdRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream, N * P, x, lBound, rBound);
    CheckVslError(errcode);

    /***** Create Summary Statistics task *****/
    errcode = vsldSSNewTask(&task, &p, &n, &x_storage, (double *)x, 0, 0);
    CheckVslError(errcode);

    /***** Edit task parameters for sorting *****/
    errcode = vsldSSEditTask(task, VSL_SS_ED_SORTED_OBSERV, y);
    CheckVslError(errcode);

    /***** Edit task parameters for sorting *****/
    errcode = vsliSSEditTask(task, VSL_SS_ED_SORTED_OBSERV_STORAGE, &sorted_x_storage);
    CheckVslError(errcode);

    /***** Sort data using radix method *****/
    errcode = vsldSSCompute(task, VSL_SS_SORTED_OBSERV, VSL_SS_METHOD_RADIX);
    CheckVslError(errcode);

    /***** Check the correctness of sorting *****/
    for( j = 0; j < p; j++ )
    {
        for ( i = 0; i < n - 1; i++ )
        {
            if ( y[j][i] > y[j][i + 1] )
            {
                errnums++;
            }
        }
    }

    /***** Printing results *****/
    printf("Task dimension : %d\n", p);
    printf("Number of observations : %d\n\n", n);

    /***** Printing of the initial matrix *****/
    printf("\n Initial matrix: \n");
    for ( j = 0; j < p; j++ )
    {
        for ( i = 0; i < n; i++ )
        {
            printf("%+.3f   ", x[j][i]);
        }
        printf("\n");
    }
    printf("\n");

    /***** Printing of the sorted matrix *****/
    printf("\n Sorted matrix: \n");
    for ( j = 0; j < p; j++ )
    {
        for ( i = 0; i < n; i++ )
        {
            printf("%+.3f   ", y[j][i]);
        }
        printf("\n");
    }

    /***** Printing summary of the test *****/
    if (errnums == 0)
    {
        printf("\n\n Sorting is correct \n\n");
    }
    else
    {
        printf("\n\n Error: Sorting is incorrect \n\n");
        return 1;
    }

    /***** Delete Summary Statistics task *****/
    errcode = vslSSDeleteTask(&task);
    CheckVslError(errcode);

    MKL_Free_Buffers();

    return 0;
}
