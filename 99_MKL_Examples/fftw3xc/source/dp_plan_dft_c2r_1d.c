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
!       Example of using fftw_plan_dft_c2r_1d function.
!
!****************************************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>
#include "fftw3.h"

static void init_c(fftw_complex *x, int N, int H);
static int verify_r(double *x, int N, int H);

int main(void)
{
    /* Size of 1D transform */
    int N = 2048;

    /* Arbitrary harmonic used to verify FFT */
    int H = -2;

    /* FFTW plan handle */
    fftw_plan c2r = 0;

    /* Pointers to input and output data */
    fftw_complex *x = 0;

    /* Execution status */
    int status = 0;

    printf("Example dp_plan_dft_c2r_1d\n");
    printf("1D complex-to-real in-place transform\n");
    printf("Configuration parameters:\n");
    printf(" N = %d\n", N);
    printf(" H = %d\n", H);

    printf("Allocate complex data array x(%i)\n", N/2+1);
    x  = fftw_malloc(sizeof(fftw_complex)*(N/2+1));
    if (0 == x) goto failed;

    printf("Create FFTW plan for 1D c2r in-place FFT\n");
    c2r = fftw_plan_dft_c2r_1d(N, x, (double*)x, FFTW_ESTIMATE);
    if (0 == c2r) goto failed;

    printf("Initialize input for c2r transform\n");
    init_c(x, N, H);

    printf("Compute c2r FFT\n");
    fftw_execute(c2r);

    printf("Verify the result of forward FFT\n");
    status = verify_r((double*)x, N, H);
    if (0 != status) goto failed;

 cleanup:

    printf("Destroy FFTW plan\n");
    fftw_destroy_plan(c2r);

    printf("Free data array\n");
    fftw_free(x);

    printf("TEST %s\n",0==status ? "PASSED" : "FAILED");
    return status;

 failed:
    printf(" ERROR\n");
    status = 1;
    goto cleanup;
}

/* Compute (K*L)%M accurately */
static double moda(int K, int L, int M)
{
    return (double)(((long long)K * L) % M);
}

/* Initialize array x(N) to produce unit peak at y(H) */
static void init_c(fftw_complex *x, int N, int H)
{
    double TWOPI = 6.2831853071795864769, phase;
    int n;

    for (n = 0; n < N/2+1; n++)
    {
        phase  = moda(n,H,N) / N;
        x[n][0] =  cos( TWOPI * phase ) / N;
        x[n][1] = -sin( TWOPI * phase ) / N;
    }
}

/* Verify that x has unit peak at H */
static int verify_r(double *x, int N, int H)
{
    double err, errthr, maxerr;
    int n;

    /*
     * Note, this simple error bound doesn't take into account error of
     * input data
     */
    errthr = 2.5 * log( (double)N ) / log(2.0) * DBL_EPSILON;
    printf(" Check if err is below errthr %.3lg\n", errthr);

    maxerr = 0;
    for (n = 0; n < N; n++)
    {
        double re_exp = 0.0, re_got;

        if ((n-H)%N == 0)
        {
            re_exp = 1;
        }

        re_got = x[n];
        err  = fabs(re_got - re_exp);
        if (err > maxerr) maxerr = err;
        if (!(err < errthr))
        {
            printf(" x[%i]: ",n);
            printf(" expected %.17lg, ",re_exp);
            printf(" got %.17lg, ",re_got);
            printf(" err %.3lg\n", err);
            printf(" Verification FAILED\n");
            return 1;
        }
    }
    printf(" Verified,  maximum error was %.3lg\n", maxerr);
    return 0;
}
