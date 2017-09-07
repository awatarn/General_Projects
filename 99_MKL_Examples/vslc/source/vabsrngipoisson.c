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
!    viRngPoisson, abstract BRNG  Example Program Text
!******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mkl_vsl.h"
#include "errcheck.inc"


#define ABSTR_BRNG    VSL_BRNG_SABSTRACT

#define BRNG          VSL_BRNG_MCG31
#define SEED           123
#define METHOD           0
#define N               25
#define BUFN            10
#define PRINTN          10

#define M 0x7FFFFFFF /* 2^31-1 */

int MysUpdateFunc(VSLStreamStatePtr , int* , float [], int* , int* , int* );

static FILE* fp;

int main()
{
    int i, dif, errcode;
    float lambda;
    float a, b;
    float fBuffer[BUFN];
    int r[N], rCpy[N];
    char filename[100];
    VSLStreamStatePtr stream, streamAbstr;

    scanf("%99s", filename);

    /* File with external integer numbers
       (integer output of MCG31 initialized with seed=123 */
    if ( (fp = fopen( filename, "r" )) == NULL )
    {
        printf("Open file error\n");
        exit(1);
    }

    /* interval boundaries of uniform distribution */
    a = 0.0;
    b = 1.0;

    /* Parameter of Poisson distribution */
    lambda = 30.0;

    /***** Initialize streams *****/
    errcode = vslNewStream( &stream, BRNG,  SEED );
    CheckVslError( errcode );
    errcode = vslsNewAbstractStream( &streamAbstr, BUFN, fBuffer, a, b, MysUpdateFunc );
    CheckVslError( errcode );

    /***** Call RNG *****/
    errcode = viRngPoisson( VSL_RNG_METHOD_POISSON_PTPE, stream, N, r, lambda );
    CheckVslError( errcode );
    errcode = viRngPoisson( VSL_RNG_METHOD_POISSON_PTPE, streamAbstr, N, rCpy, lambda );
    CheckVslError( errcode );

    /***** Compare results *****/
    printf("Abstract BRNG for single precision arrays was used for\n");
    printf("generation of rCpy array of Poisson distributed random numbers\n\n");

    dif = 0;
    for( i = 0; i < N; i++ )
    {
        if ( r[i] != rCpy[i] )
        {
            dif++;
        }
    }

    /***** Printing results *****/
    if ( dif == 0 )
    {
        printf("Both arrays are identical\n");
    }
    else
    {
        printf("There are %d different elements in the arrays\n", dif);
    }

    printf("---------------------------\n\n");
    printf("First %d elements from both arrays\n", PRINTN);
    printf("----------------------------------\n");
    printf("array r array rCpy\n");
    printf("------------------\n");

    for ( i = 0; i < PRINTN; i++ )
    {
        printf("%d          %d\n", r[i], rCpy[i]);
    }

    printf("\n");

    errcode = vslDeleteStream( &stream );
    CheckVslError( errcode );
    errcode = vslDeleteStream( &streamAbstr );
    CheckVslError( errcode );

    fclose(fp);
    return 0;
}

/* Callback function used in initialization of abstract stream */
int MysUpdateFunc(VSLStreamStatePtr stream, int* n, float buf[], int* nmin, int* nmax, int* idx)
{
    int i, errcode;
    unsigned int num;
    float c;

    c = 1.0 / (float)M;

    for ( i = 0; i < *nmax; i++ )
    {
        if (fscanf(fp, "%u", &num) == EOF) break;
        buf[(*idx+i) % (*n)] = (float)num * c;
    }

    return i;
}
