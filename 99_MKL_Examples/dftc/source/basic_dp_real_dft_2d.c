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
! A simple example of double-precision real-to-complex out-of-place 2D
! FFT using Intel(R) MKL DFTI
!
!****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "mkl_service.h"
#include "mkl_dfti.h"

static void init_r(double *x, int N1, int N2, int H1, int H2);
static void init_c(MKL_Complex16 *x, int N1, int N2, int H1, int H2);
static int verify_c(MKL_Complex16 *x, int N1, int N2, int H1, int H2);
static int verify_r(double *x, int N1, int N2, int H1, int H2);

/* Define the format to printf MKL_LONG values */
#if !defined(MKL_ILP64)
#define LI "%li"
#else
#define LI "%lli"
#endif

int main(void)
{
    /* Size of 2D transform */
    int N1 = 4, N2 = 5;

    /* Arbitrary harmonic used to verify FFT */
    int H1 = -1, H2 = 2;

    /* Execution status */
    MKL_LONG status = 0;

    /* Pointers to input and output data */
    double *x_real = 0;
    MKL_Complex16 *x_cmplx = 0;

    DFTI_DESCRIPTOR_HANDLE hand = 0;

    char version[DFTI_VERSION_LENGTH];

    DftiGetValue(0, DFTI_VERSION, version);
    printf("%s\n", version);

    printf("Example basic_dp_real_dft_2d\n");
    printf("Forward-Backward double-precision 2D real out-of-place FFT\n");
    printf("Configuration parameters:\n");
    printf(" DFTI_PRECISION                = DFTI_DOUBLE\n");
    printf(" DFTI_FORWARD_DOMAIN           = DFTI_REAL\n");
    printf(" DFTI_DIMENSION                = 2\n");
    printf(" DFTI_LENGTHS                  = {%i, %i}\n", N1, N2);
    printf(" DFTI_PLACEMENT                = DFTI_NOT_INPLACE\n");
    printf(" DFTI_CONJUGATE_EVEN_STORAGE   = DFTI_COMPLEX_COMPLEX\n");
    /*printf(" DFTI_PACKED_FORMAT            = DFTI_CCE_FORMAT\n");*/

    printf("Create DFTI descriptor\n");
    {
        MKL_LONG N[2];  N[0] = N1;  N[1] = N2;
        status = DftiCreateDescriptor(&hand, DFTI_DOUBLE, DFTI_REAL, 2, N);
        if (0 != status) goto failed;
    }

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

    printf("Set input  strides = ");
    {
        MKL_LONG rs[3]; rs[0] = 0; rs[1] = N2; rs[2] = 1;
        printf("{"LI", "LI", "LI"}\n", rs[0],rs[1],rs[2]);
        status = DftiSetValue(hand, DFTI_INPUT_STRIDES, rs);
        if (0 != status) goto failed;
    }

    printf("Set output strides = ");
    {
        MKL_LONG cs[3]; cs[0] = 0; cs[1] = N2/2+1; cs[2] = 1;
        printf("{"LI", "LI", "LI"}\n", cs[0],cs[1],cs[2]);
        status = DftiSetValue(hand, DFTI_OUTPUT_STRIDES, cs);
        if (0 != status) goto failed;
    }

    printf("Commit the descriptor\n");
    status = DftiCommitDescriptor(hand);
    if (0 != status) goto failed;

    printf("Allocate data arrays\n");
    x_real = (double*)mkl_malloc(N1*N2*sizeof(double), 64);
    if (0 == x_real) goto failed;
    x_cmplx = (MKL_Complex16*)mkl_malloc(N1*(N2/2+1)*sizeof(MKL_Complex16), 64);
    if (0 == x_cmplx) goto failed;

    printf("Initialize data for r2c transform\n");
    init_r(x_real, N1, N2, H1, H2);

    printf("Compute real-to-complex transform\n");
    status = DftiComputeForward(hand, x_real, x_cmplx);
    if (0 != status) goto failed;

    printf("Verify the result\n");
    status = verify_c(x_cmplx, N1, N2, H1, H2);
    if (0 != status) goto failed;

    printf("Reconfigure DFTI descriptor for backward transform\n");

    printf("Set input  strides = ");
    {
        MKL_LONG cs[3]; cs[0] = 0; cs[1] = N2/2+1; cs[2] = 1;
        printf("{"LI", "LI", "LI"}\n", cs[0],cs[1],cs[2]);
        status = DftiSetValue(hand, DFTI_INPUT_STRIDES, cs);
        if (0 != status) goto failed;
    }

    printf("Set output strides = ");
    {
        MKL_LONG rs[3]; rs[0] = 0; rs[1] = N2; rs[2] = 1;
        printf("{"LI", "LI", "LI"}\n", rs[0],rs[1],rs[2]);
        status = DftiSetValue(hand, DFTI_OUTPUT_STRIDES, rs);
        if (0 != status) goto failed;
    }

    printf("Commit the descriptor\n");
    status = DftiCommitDescriptor(hand);
    if (0 != status) goto failed;

    printf("Initialize data for c2r transform\n");
    init_c(x_cmplx, N1, N2, H1, H2);

    printf("Compute backward transform\n");
    status = DftiComputeBackward(hand, x_cmplx, x_real);
    if (0 != status) goto failed;

    printf("Verify the result of the backward transform\n");
    status = verify_r(x_real, N1, N2, H1, H2);
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

/* Initialize array x(N) to produce unit peaks at x(H) and x(N-H) */
static void init_r(double *x, int N1, int N2, int H1, int H2)
{
    double TWOPI = 6.2831853071795864769, phase, factor;
    int n1, n2, S1, S2, index;

    /* Generalized strides for row-major addressing of x */
    S2 = 1;
    S1 = N2;

    factor = (2*(N1-H1)%N1==0 && 2*(N2-H2)%N2==0) ? 1.0 : 2.0;
    for (n1 = 0; n1 < N1; n1++)
    {
        for (n2 = 0; n2 < N2; n2++)
        {
            phase  = moda(n1,H1,N1) / N1;
            phase += moda(n2,H2,N2) / N2;
            index = n1*S1 + n2*S2;
            x[index] = factor * cos( TWOPI * phase ) / (N1*N2);
        }
    }
}

/* Verify that x has unit peak at H */
static int verify_c(MKL_Complex16 *x, int N1, int N2, int H1, int H2)
{
    double err, errthr, maxerr;
    int n1, n2, S1, S2, index;

    /* Generalized strides for row-major addressing of x */
    S2 = 1;
    S1 = N2/2+1;

    /*
     * Note, this simple error bound doesn't take into account error of
     * input data
     */
    errthr = 2.5 * log( (double)N1*N2 ) / log(2.0) * DBL_EPSILON;
    printf(" Check if err is below errthr %.3lg\n", errthr);

    maxerr = 0;
    for (n1 = 0; n1 < N1; n1++)
    {
        for (n2 = 0; n2 < N2/2+1; n2++)
        {
            double re_exp = 0.0, im_exp = 0.0, re_got, im_got;

            if ((( n1-H1)%N1==0 && ( n2-H2)%N2==0) ||
                ((-n1-H1)%N1==0 && (-n2-H2)%N2==0))
            {
                re_exp = 1;
            }

            index = n1*S1 + n2*S2;
            re_got = x[index].real;
            im_got = x[index].imag;
            err  = fabs(re_got - re_exp) + fabs(im_got - im_exp);
            if (err > maxerr) maxerr = err;
            if (!(err < errthr))
            {
                printf(" x[%i][%i]: ",n1,n2);
                printf(" expected (%.17lg,%.17lg), ",re_exp,im_exp);
                printf(" got (%.17lg,%.17lg), ",re_got,im_got);
                printf(" err %.3lg\n", err);
                printf(" Verification FAILED\n");
                return 1;
            }
        }
    }
    printf(" Verified,  maximum error was %.3lg\n", maxerr);
    return 0;
}

/* Initialize array x(N) to produce unit peak at x(H) */
static void init_c(MKL_Complex16 *x, int N1, int N2, int H1, int H2)
{
    double TWOPI = 6.2831853071795864769, phase;
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
            x[index].real =  cos( TWOPI * phase ) / (N1*N2);
            x[index].imag = -sin( TWOPI * phase ) / (N1*N2);
        }
    }
}

/* Verify that x has unit peak at H */
static int verify_r(double *x, int N1, int N2, int H1, int H2)
{
    double err, errthr, maxerr;
    int n1, n2, S1, S2, index;

    /* Generalized strides for row-major addressing of x */
    S2 = 1;
    S1 = N2;

    /*
     * Note, this simple error bound doesn't take into account error of
     * input data
     */
    errthr = 2.5 * log( (double)N1*N2 ) / log(2.0) * DBL_EPSILON;
    printf(" Check if err is below errthr %.3lg\n", errthr);

    maxerr = 0;
    for (n1 = 0; n1 < N1; n1++)
    {
        for (n2 = 0; n2 < N2; n2++)
        {
            double re_exp = 0.0, re_got;

            if ((n1-H1)%N1==0 && (n2-H2)%N2==0)
            {
                re_exp = 1;
            }

            index = n1*S1 + n2*S2;
            re_got = x[index];
            err  = fabs(re_got - re_exp);
            if (err > maxerr) maxerr = err;
            if (!(err < errthr))
            {
                printf(" x[%i][%i]: ",n1,n2);
                printf(" expected %.17lg, ",re_exp);
                printf(" got %.17lg, ",re_got);
                printf(" err %.3lg\n", err);
                printf(" Verification FAILED\n");
                return 1;
            }
        }
    }
    printf(" Verified,  maximum error was %.3lg\n", maxerr);
    return 0;
}
