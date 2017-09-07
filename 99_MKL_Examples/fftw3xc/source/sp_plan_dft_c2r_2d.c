/*******************************************************************************
* Copyright 2011-2017 Intel Corporation All Rights Reserved.
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
! Content:
!       Example of using fftwf_plan_dft_c2r_2d function.
!
!****************************************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>
#include "fftw3.h"

static void init_c(fftwf_complex *x, int N1, int N2, int H1, int H2);
static int verify_r(float *x, int N1, int N2, int H1, int H2);

int main(void)
{
    /* Sizes of 2D transform */
    int N1 = 20;
    int N2 = 25;

    /* Arbitrary harmonic used to verify FFT */
    int H1 = N1/2;
    int H2 = -2;

    /* FFTW plan handles */
    fftwf_plan c2r = 0;

    /* Pointers to input and output data */
    fftwf_complex *x = 0;
    float *y = 0;

    /* Execution status */
    int status = 0;

    printf("Example sp_plan_dft_c2r_2d\n");
    printf("2D real-to-complex out-of-place transform\n");
    printf("Configuration parameters:\n");
    printf(" N = {%d, %d}\n", N1, N2);
    printf(" H = {%d, %d}\n", H1, H2);

    printf("Allocate data arrays: complex x(%i), real y(%i)\n", N1*(N2/2+1), N1*N2);
    x  = fftwf_malloc(sizeof(fftwf_complex)*N1*(N2/2+1));
    y  = fftwf_malloc(sizeof(float)*N1*N2);
    if (0 == x || 0 == y) goto failed;

    printf("Create FFTW plan for 2D c2r out-of-place FFT\n");
    c2r = fftwf_plan_dft_c2r_2d(N1, N2, x, y, FFTW_ESTIMATE);
    if (0 == c2r) goto failed;

    printf("Initialize input for c2r transform\n");
    init_c(x, N1, N2, H1, H2);

    printf("Compute c2r FFT\n");
    fftwf_execute(c2r);

    printf("Verify the result of c2r FFT\n");
    status = verify_r(y, N1, N2, H1, H2);
    if (0 != status) goto failed;

 cleanup:

    printf("Destroy FFTW plan\n");
    fftwf_destroy_plan(c2r);

    printf("Free data array\n");
    fftwf_free(x);
    fftwf_free(y);

    printf("TEST %s\n",0==status ? "PASSED" : "FAILED");
    return status;

 failed:
    printf(" ERROR\n");
    status = 1;
    goto cleanup;
}

/* Compute (K*L)%M accurately */
static float moda(int K, int L, int M)
{
    return (float)(((long long)K * L) % M);
}

/* Initialize array x(N) to produce unit peak at x(H) */
static void init_c(fftwf_complex *x, int N1, int N2, int H1, int H2)
{
    float TWOPI = 6.2831853071795864769f, phase;
    int n1, n2, S1, S2, index;

    /* Generalized strides for row-major addressing of x */
    S2 = 1;
    S1 = N2/2+1;

    for (n1 = 0; n1 < N1; n1++)
    {
        for (n2 = 0; n2 < N2/2+1; n2++)
        {
            phase  = moda(n1,H1,N1) / N1;
            phase += moda(n2,H2,N2) / N2;
            index = n1*S1 + n2*S2;
            x[index][0] =  cosf( TWOPI * phase ) / (N1*N2);
            x[index][1] = -sinf( TWOPI * phase ) / (N1*N2);
        }
    }
}

/* Verify that x has unit peak at H */
static int verify_r(float *x, int N1, int N2, int H1, int H2)
{
    float err, errthr, maxerr;
    int n1, n2, S1, S2, index;

    /* Generalized strides for row-major addressing of x */
    S2 = 1;
    S1 = N2;

    /*
     * Note, this simple error bound doesn't take into account error of
     * input data
     */
    errthr = 2.5f * logf( (float)N1*N2 ) / logf(2.0f) * FLT_EPSILON;
    printf(" Check if err is below errthr %.3g\n", errthr);

    maxerr = 0;
    for (n1 = 0; n1 < N1; n1++)
    {
        for (n2 = 0; n2 < N2; n2++)
        {
            float re_exp = 0.0, re_got;

            if ((n1-H1)%N1==0 && (n2-H2)%N2==0)
            {
                re_exp = 1;
            }

            index = n1*S1 + n2*S2;
            re_got = x[index];
            err  = fabsf(re_got - re_exp);
            if (err > maxerr) maxerr = err;
            if (!(err < errthr))
            {
                printf(" x[%i][%i]: ",n1,n2);
                printf(" expected %.7g, ",re_exp);
                printf(" got %.7g, ",re_got);
                printf(" err %.3g\n", err);
                printf(" Verification FAILED\n");
                return 1;
            }
        }
    }
    printf(" Verified,  maximum error was %.3g\n", maxerr);
    return 0;
}
