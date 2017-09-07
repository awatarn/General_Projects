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
! A simple example of double-precision real-to-complex out-of-place 1D
! FFT using Intel(R) MKL DFTI
!
!****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "mkl_service.h"
#include "mkl_dfti.h"

static void init_r(double *x, int N, int H);
static int verify_c(MKL_Complex16 *x, int N, int H);
static void init_c(MKL_Complex16 *x, int N, int H);
static int verify_r(double *x, int N, int H);

/* Define the format to printf MKL_LONG values */
#if !defined(MKL_ILP64)
#define LI "%li"
#else
#define LI "%lli"
#endif

int main(void)
{
    /* Size of 1D transform */
    const int N = 6;

    /* Arbitrary harmonic used to verify FFT */
    int H = -1;

    /* Execution status */
    MKL_LONG status = 0;

    /* Pointers to input and output data */
    double *x_real = 0;
    MKL_Complex16 *x_cmplx = 0;

    DFTI_DESCRIPTOR_HANDLE hand = 0;

    char version[DFTI_VERSION_LENGTH];

    DftiGetValue(0, DFTI_VERSION, version);
    printf("%s\n", version);

    printf("Example basic_dp_real_dft_1d\n");
    printf("Forward-Backward double-precision real-to-complex"
           " out-of-place 1D transform\n");
    printf("Configuration parameters:\n");
    printf(" DFTI_PRECISION                = DFTI_DOUBLE\n");
    printf(" DFTI_FORWARD_DOMAIN           = DFTI_REAL\n");
    printf(" DFTI_DIMENSION                = 1\n");
    printf(" DFTI_LENGTHS                  = {%d}\n", N);
    printf(" DFTI_PLACEMENT                = DFTI_NOT_INPLACE\n");
    printf(" DFTI_CONJUGATE_EVEN_STORAGE   = DFTI_COMPLEX_COMPLEX\n");
    /*printf(" DFTI_PACKED_FORMAT            = DFTI_CCE_FORMAT\n");*/


    printf("Create DFTI descriptor\n");
    status = DftiCreateDescriptor(&hand, DFTI_DOUBLE, DFTI_REAL,
                                  1, (MKL_LONG)N);
    if (0 != status) goto failed;

    printf("Set configuration: out-of-place\n");
    status = DftiSetValue(hand, DFTI_PLACEMENT, DFTI_NOT_INPLACE);
    if (0 != status) goto failed;

    printf("Set configuration: CCE storage\n");
    status = DftiSetValue(hand, DFTI_CONJUGATE_EVEN_STORAGE,
                          DFTI_COMPLEX_COMPLEX);
    if (0 != status) goto failed;

    /* This is not needed for DFTI_COMPLEX_COMPLEX storage */
    /* status = DftiSetValue(hand, DFTI_PACKED_FORMAT, DFTI_CCE_FORMAT); */
    /* if (0 != status) goto failed; */

    printf("Commit the  descriptor\n");
    status = DftiCommitDescriptor(hand);
    if (0 != status) goto failed;

    printf("Allocate data arrays\n");
    x_real  = (double*)mkl_malloc(N*sizeof(double), 64);
    x_cmplx = (MKL_Complex16*)mkl_malloc((N/2+1)*sizeof(MKL_Complex16), 64);
    if (0 == x_real || 0 == x_cmplx) goto failed;

    printf("Initialize data for real-to-complex FFT\n");
    init_r(x_real, N, H);

    printf("Compute forward transform\n");
    status = DftiComputeForward(hand, x_real, x_cmplx);
    if (0 != status) goto failed;

    printf("Verify the result\n");
    status = verify_c(x_cmplx, N, H);
    if (0 != status) goto failed;

    printf("Initialize data for complex-to-real FFT\n");
    init_c(x_cmplx, N, H);

    printf("Compute backward transform\n");
    status = DftiComputeBackward(hand, x_cmplx, x_real);
    if (0 != status) goto failed;

    printf("Verify the result\n");
    status = verify_r(x_real, N, H);
    if (0 != status) goto failed;

 cleanup:

    printf("Free DFTI descriptor\n");
    DftiFreeDescriptor(&hand);

    printf("Free data arrays\n");
    mkl_free(x_real);
    mkl_free(x_cmplx);

    printf("TEST %s\n",0==status ? "PASSED" : "FAILED");
    return status;

 failed:
    printf(" ERROR, status = "LI"\n", status);
    status = 1;
    goto cleanup;
}

/* Compute (K*L)%M accurately */
static double moda(int K, int L, int M)
{
    return (double)(((long long)K * L) % M);
}

/* Initialize array x(N) to produce unit peaks at y(H) and y(N-H) */
static void init_r(double *x, int N, int H)
{
    double TWOPI = 6.2831853071795864769, phase, factor;
    int n;

    factor = (2*(N-H)%N == 0) ? 1.0 : 2.0;
    for (n = 0; n < N; n++)
    {
        phase  = moda(n,H,N) / N;
        x[n] = factor * cos( TWOPI * phase ) / N;
    }
}

/* Verify that x has unit peak at H */
static int verify_c(MKL_Complex16 *x, int N, int H)
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
    for (n = 0; n < N/2+1; n++)
    {
        double re_exp = 0.0, im_exp = 0.0, re_got, im_got;

        if ((n-H)%N == 0 || (-n-H)%N == 0)
        {
            re_exp = 1;
        }

        re_got = x[n].real;
        im_got = x[n].imag;
        err  = fabs(re_got - re_exp) + fabs(im_got - im_exp);
        if (err > maxerr) maxerr = err;
        if (!(err < errthr))
        {
            printf(" x[%i]: ",n);
            printf(" expected (%.17lg,%.17lg), ",re_exp,im_exp);
            printf(" got (%.17lg,%.17lg), ",re_got,im_got);
            printf(" err %.3lg\n", err);
            printf(" Verification FAILED\n");
            return 1;
        }
    }
    printf(" Verified,  maximum error was %.3lg\n", maxerr);
    return 0;
}

/* Initialize array x(N) to produce unit peak at y(H) */
static void init_c(MKL_Complex16 *x, int N, int H)
{
    double TWOPI = 6.2831853071795864769, phase;
    int n;

    for (n = 0; n < N/2+1; n++)
    {
        phase  = moda(n,H,N) / N;
        x[n].real =  cos( TWOPI * phase ) / N;
        x[n].imag = -sin( TWOPI * phase ) / N;
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
