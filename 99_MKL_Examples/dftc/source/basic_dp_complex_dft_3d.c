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
! A simple example of double-precision complex-to-complex in-place 3D
! FFT using Intel(R) MKL DFTI
!
!****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "mkl_service.h"
#include "mkl_dfti.h"

static void init(MKL_Complex16 *x, int N1, int N2, int N3,
                 int H1, int H2, int H3);
static int verify(MKL_Complex16 *x, int N1, int N2, int N3,
                  int H1, int H2, int H3);

/* Define the format to printf MKL_LONG values */
#if !defined(MKL_ILP64)
#define LI "%li"
#else
#define LI "%lli"
#endif

int main(void)
{
    /* Sizes of 3D transform */
    int N1 = 7, N2 = 13, N3 = 5;

    /* Arbitrary harmonic used to verify FFT */
    int H1 = -2, H2 = -3, H3 = -4;

    /* Pointer to input/output data */
    MKL_Complex16 *x = 0;

    /* Execution status */
    MKL_LONG status = 0;

    DFTI_DESCRIPTOR_HANDLE hand = 0;

    char version[DFTI_VERSION_LENGTH];

    DftiGetValue(0, DFTI_VERSION, version);
    printf("%s\n", version);

    printf("Example basic_dp_complex_dft_3d\n");
    printf("Forward and backward double-precision complex 3D transforms\n");
    printf("Configuration parameters:\n");
    printf(" DFTI_PRECISION      = DFTI_DOUBLE\n");
    printf(" DFTI_FORWARD_DOMAIN = DFTI_COMPLEX\n");
    printf(" DFTI_DIMENSION      = 3\n");
    printf(" DFTI_LENGTHS        = {%i, %i, %i}\n", N1, N2, N3);


    printf("Create DFTI descriptor\n");
    {
        MKL_LONG N[3]; N[0] = N1; N[1] = N2; N[2] = N3;
        status = DftiCreateDescriptor(&hand, DFTI_DOUBLE, DFTI_COMPLEX, 3, N);
        if (0 != status) goto failed;
    }

    printf("Commit DFTI descriptor\n");
    status = DftiCommitDescriptor(hand);
    if (0 != status) goto failed;

    printf("Allocate input array\n");
    x = (MKL_Complex16*)mkl_malloc(N1*N2*N3*sizeof(MKL_Complex16), 64);
    if (0 == x) goto failed;

    printf("Initialize input for forward transform\n");
    init(x, N1, N2, N3, H1, H2, H3);

    printf("Compute forward transform\n");
    status = DftiComputeForward(hand, x);
    if (0 != status) goto failed;

    printf("Verify the result of forward FFT\n");
    status = verify(x, N1, N2, N3, H1, H2, H3);
    if (0 != status) goto failed;

    printf("Initialize input for backward transform\n");
    init(x, N1, N2, N3, -H1, -H2, -H3);

    printf("Compute backward transform\n");
    status = DftiComputeBackward(hand, x);
    if (0 != status) goto failed;

    printf("Verify the result of backward FFT\n");
    status = verify(x, N1, N2, N3, H1, H2, H3);
    if (0 != status) goto failed;

 cleanup:

    printf("Free DFTI descriptor\n");
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
static double moda(int K, int L, int M)
{
    return (double)(((long long)K * L) % M);
}

/* Initialize array with harmonic {H1, H2, H3} */
static void init(MKL_Complex16 *x, int N1, int N2, int N3,
                 int H1, int H2, int H3)
{
    double TWOPI = 6.2831853071795864769, phase;
    int n1, n2, n3, index;

    /* Generalized strides for row-major addressing of x */
    int S1 = N2*N3, S2 = N3, S3 = 1;

    for (n1 = 0; n1 < N1; n1++)
    {
        for (n2 = 0; n2 < N2; n2++)
        {
            for (n3 = 0; n3 < N3; n3++)
            {
                phase =  moda(n1,H1,N1) / N1;
                phase += moda(n2,H2,N2) / N2;
                phase += moda(n3,H3,N3) / N3;
                index = n1*S1 + n2*S2 + n3*S3;
                x[index].real = cos( TWOPI * phase ) / (N1*N2*N3);
                x[index].imag = sin( TWOPI * phase ) / (N1*N2*N3);
            }
        }
    }
}

/* Verify that x(n1,n2,n3) is a peak at H1,H2,H3 */
static int verify(MKL_Complex16 *x, int N1, int N2, int N3,
                  int H1, int H2, int H3)
{
    double err, errthr, maxerr;
    int n1, n2, n3, index;

    /* Generalized strides for row-major addressing of x */
    int S1 = N2*N3, S2 = N3, S3 = 1;

    /*
     * Note, this simple error bound doesn't take into account error of
     * input data
     */
    errthr = 5.0 * log( (double)N1*N2*N3 ) / log(2.0) * DBL_EPSILON;
    printf(" Verify the result, errthr = %.3lg\n", errthr);

    maxerr = 0;
    for (n1 = 0; n1 < N1; n1++)
    {
        for (n2 = 0; n2 < N2; n2++)
        {
            for (n3 = 0; n3 < N3; n3++)
            {
                double re_exp = 0.0, im_exp = 0.0, re_got, im_got;

                if ((n1-H1)%N1==0 && (n2-H2)%N2==0 && (n3-H3)%N3==0)
                {
                    re_exp = 1;
                }

                index = n1*S1 + n2*S2 + n3*S3;
                re_got = x[index].real;
                im_got = x[index].imag;
                err  = fabs(re_got - re_exp) + fabs(im_got - im_exp);
                if (err > maxerr) maxerr = err;
                if (!(err < errthr))
                {
                    printf(" x[%i][%i][%i]: ",n1,n2,n3);
                    printf(" expected (%.17lg,%.17lg), ",re_exp,im_exp);
                    printf(" got (%.17lg,%.17lg), ",re_got,im_got);
                    printf(" err %.3lg\n", err);
                    printf(" Verification FAILED\n");
                    return 1;
                }
            }
        }
    }
    printf(" Verified, maximum error was %.3lg\n", maxerr);
    return 0;
}
