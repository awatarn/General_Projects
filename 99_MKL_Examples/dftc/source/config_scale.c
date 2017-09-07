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
! An example of using configuration parameters DFTI_FORWARD_SCALE and
! DFTI_BACKWARD_SCALE. Default value for each of these parameters is 1.0.
!
!****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "mkl_service.h"
#include "mkl_dfti.h"

static void init(MKL_Complex16 *x, MKL_LONG N, MKL_LONG H, double scale);
static int verify(MKL_Complex16 *x, MKL_LONG N, MKL_LONG H);

/* Define the format to printf MKL_LONG values */
#if !defined(MKL_ILP64)
#define LI "%li"
#else
#define LI "%lli"
#endif

int main(void)
{
    /* Size of 1D transform */
    MKL_LONG N = 1024;

    /* Arbitrary harmonic used to verify FFT */
    MKL_LONG H = -N/2;

    /* Pointer to input/output data */
    MKL_Complex16 *x = 0;

    /* Execution status */
    MKL_LONG status = 0;

    /* Forward and backward scale factors */
    double fscale, bscale;

    DFTI_DESCRIPTOR_HANDLE hand = 0;

    char version[DFTI_VERSION_LENGTH];

    DftiGetValue(0, DFTI_VERSION, version);
    printf("%s\n", version);

    printf("Example config_scale\n");
    printf("Forward and backward FFT with scaling\n");
    printf("Configuration parameters:\n");
    printf(" DFTI_PRECISION      = DFTI_DOUBLE\n");
    printf(" DFTI_FORWARD_DOMAIN = DFTI_COMPLEX\n");
    printf(" DFTI_DIMENSION      = 1\n");
    printf(" DFTI_LENGTHS        = "LI"\n", N);

    printf("Create DFTI descriptor\n");
    status = DftiCreateDescriptor(&hand, DFTI_DOUBLE, DFTI_COMPLEX, 1, N);
    if (0 != status) goto failed;

    /* Arbitrary scaling factors may be used */
    fscale = 1.0 / N;
    bscale = 1.0 / sqrt( (double)N );

    printf(" Set forward  scale = %lg\n",fscale);
    status = DftiSetValue(hand, DFTI_FORWARD_SCALE, fscale);
    if (0 != status) goto failed;

    printf(" Set backward scale = %lg\n",bscale);
    status = DftiSetValue(hand, DFTI_BACKWARD_SCALE, bscale);
    if (0 != status) goto failed;

    printf("Commit DFTI descriptor\n");
    status = DftiCommitDescriptor(hand);
    if (0 != status) goto failed;

    printf("Allocate input/output array\n");
    x = (MKL_Complex16*)mkl_malloc(N*sizeof(MKL_Complex16), 64);
    if (0 == x) goto failed;

    printf("Initialize input for forward transform\n");
    init(x, N, H, fscale);

    printf("Compute forward transform\n");
    status = DftiComputeForward(hand, x);
    if (0 != status) goto failed;

    printf("Verify the result of forward FFT\n");
    status = verify(x, N, H);
    if (0 != status) goto failed;

    printf("Initialize input for backward transform\n");
    init(x, N, -H, bscale);

    printf("Compute backward transform\n");
    status = DftiComputeBackward(hand, x);
    if (0 != status) goto failed;

    printf("Verify the result of backward FFT\n");
    status = verify(x, N, H);
    if (0 != status) goto failed;

 cleanup:

    printf("Release the DFTI descriptor\n");
    DftiFreeDescriptor(&hand);

    printf("Free data array\n");
    mkl_free(x);

    printf("TEST %s\n",0==status ? "PASSED" : "FAILED");
    return status;

 failed:
    printf(" ERROR, status = "LI"\n", status);
    status = 1;
    goto cleanup;
}

/* Compute (K*L)%M accurately */
static double moda(MKL_LONG K, MKL_LONG L, MKL_LONG M)
{
    return (double)(((long long)K * L) % M);
}

/* Initialize array with harmonic H */
static void init(MKL_Complex16 *x, MKL_LONG N, MKL_LONG H, double scale)
{
    double TWOPI = 6.2831853071795864769, phase;
    MKL_LONG n;

    for (n = 0; n < N; n++)
    {
        phase  = moda(n,H,N) / N;
        x[n].real = cos( TWOPI * phase ) / (N * scale);
        x[n].imag = sin( TWOPI * phase ) / (N * scale);
    }
}

/* Verify that x has unit peak at H */
static int verify(MKL_Complex16 *x, MKL_LONG N, MKL_LONG H)
{
    double err, errthr, maxerr;
    MKL_LONG n;

    /*
     * Note, this simple error bound doesn't take into account error of
     * input data
     */
    errthr = 5.0 * log( (double)N ) / log(2.0) * DBL_EPSILON;
    printf(" Verify the result, errthr = %.3lg\n", errthr);

    maxerr = 0;
    for (n = 0; n < N; n++)
    {
        double re_exp = 0.0, im_exp = 0.0, re_got, im_got;

        if ((n-H)%N==0)
        {
            re_exp = 1;
        }

        re_got = x[n].real;
        im_got = x[n].imag;
        err  = fabs(re_got - re_exp) + fabs(im_got - im_exp);
        if (err > maxerr) maxerr = err;
        if (!(err < errthr))
        {
            printf(" x["LI"]: ",n);
            printf(" expected (%.17lg,%.17lg), ",re_exp,im_exp);
            printf(" got (%.17lg,%.17lg), ",re_got,im_got);
            printf(" err %.3lg\n", err);
            printf(" Verification FAILED\n");
            return 100;
        }
    }
    printf(" Verified, maximum error was %.3lg\n", maxerr);
    return 0;
}
