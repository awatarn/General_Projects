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
!    Parametrization of correlation matrix  Example Program Text
!******************************************************************************/

#include <stdio.h>

#include "mkl.h"
#include "errcheck.inc"

#define DIM   3          /* Task dimension */
#define LWORK 2*25*DIM

#define TEST_THRESHOLD -1.0E-6

/* Distorted correlation matrix */
float cor[DIM][DIM] = {
    {  1.0,  0.95,  0.7 },
    { 0.95,   1.0, 0.29 },
    {  0.7,  0.29,  1.0 }
};

int main()
{
    VSLSSTaskPtr task;
    MKL_INT dim;
    MKL_INT cor_storage;
    MKL_INT pcor_storage;
    float p_cor[DIM][DIM];
    float copy_cor[DIM][DIM];
    int i, j, errcode;
    int errnums = 0;

    /***** Following variables are used in routine which finds eigenvalues
           of simmetric matrix *****/
    float eigenvals[DIM], work[LWORK];
    MKL_INT lwork, info;
    char jobz, uplo;

    /***** Initializing parameters for Summary Statistics task *****/
    dim          = DIM;
    cor_storage  = VSL_SS_MATRIX_STORAGE_FULL;
    pcor_storage = VSL_SS_MATRIX_STORAGE_FULL;

    /***** Create Summary Statistics task *****/
    errcode = vslsSSNewTask( &task, &dim, 0, 0, 0, 0, 0 );
    CheckVslError(errcode);

    /***** Edit task parameters for parameterization of correlation *****/
    errcode = vslsSSEditCorParameterization( task,
              (float*)cor, &cor_storage, (float*)p_cor, &pcor_storage );
    CheckVslError(errcode);

    /***** Parametrize correlation *****/
    errcode = vslsSSCompute( task, VSL_SS_PARAMTR_COR,
                             VSL_SS_METHOD_SD );
    CheckVslError(errcode);

    /***** Compute eigenvalues of distorted correlation matrix *****/
    for(i = 0; i < dim; i++)
    {
        for(j = 0; j < dim; j++)
        {
            copy_cor[i][j] = cor[i][j];
        }
    }

    lwork = LWORK;
    jobz = 'N';
    uplo = 'U';
    ssyev( &jobz, &uplo, &dim, (float*)copy_cor, &dim,
           eigenvals, work, &lwork, &info );
    CheckVslError(info);

    /***** Printing results *****/
    printf("Task dimension : %d\n", dim);

    /***** Print distorted correlation matrix and it's eigenvalues *****/
    printf("\nDistorted correlation matrix\n");
    for(i = 0; i < dim; i++)
    {
        for(j = 0; j < dim; j++)
        {
            printf("%+1.5f ", cor[i][j]);
        }
        printf("\n");
    }

    printf("\nEigenvalues of the distorted correlation matrix\n");
    for(j = 0; j < dim; j++)
    {
        printf("%+1.5f ", eigenvals[j]);
    }
    printf("\n");

    /***** Compute eigevalue of parametrized correlation matrix *****/
    for(i = 0; i < dim; i++)
    {
        for(j = 0; j < dim; j++)
        {
            copy_cor[i][j] = p_cor[i][j];
        }
    }

    lwork = LWORK;
    jobz = 'N';
    uplo = 'U';
    ssyev( &jobz, &uplo, &dim, (float*)copy_cor, &dim,
           eigenvals, work, &lwork, &info );
    CheckVslError(info);

    /***** Print parametrized correlation matrix and it's eigenvalues *****/
    printf("\nParameterized correlation matrix\n");
    for (i = 0; i < dim; i++)
    {
        for(j = 0; j < dim; j++)
        {
            printf("%+1.5f ", p_cor[i][j]);
        }
        printf("\n");
    }

    printf("\nEigenvalues of the parameterized correlation matrix\n");
    for(j = 0; j < dim; j++)
    {
        printf("%+1.5f ", eigenvals[j]);
    }
    printf("\n\n");

    for(i = 0; i < dim; i++)
    {
        if (eigenvals[i] < TEST_THRESHOLD) errnums++;
    }

    if (errnums == 0)
    {
        printf("\nAll eigenvalues of parametrized correlation are in the");
        printf(" expected range\n");
    }
    else
    {
        printf("\nError: Parameterized correlation matrix has %i", errnums);
        printf(" negative eigenvalues\n");
        return 1;
    }

    /***** Delete Summary Statistics task *****/
    errcode = vslSSDeleteTask( &task );
    CheckVslError(errcode);

    MKL_Free_Buffers();

    return 0;
}
