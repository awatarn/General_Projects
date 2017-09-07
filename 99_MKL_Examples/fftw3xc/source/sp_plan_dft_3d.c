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
!       Example of using fftwf_plan_dft_3d function.
!
!****************************************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>
#include "fftw3.h"

static void init(fftwf_complex *x,
                 int N1,int N2,int N3,
                 int H1,int H2,int H3);

static int verify(fftwf_complex *x,
                  int N1,int N2,int N3,
                  int H1,int H2,int H3);

int main(void)
{
    /* Sizes of 3D transform */
    int N[3] = {16, 16, 32};

    /* Arbitrary harmonic used to verify FFT */
    int H[3] = {-2, -3, -4};

    /* FFTW plan handles */
    fftwf_plan forward_plan = 0, backward_plan = 0;

    /* Pointer to input/output data */
    fftwf_complex *x = 0;

    /* Execution status */
    int status = 0;

    printf("Example sp_plan_dft_3d\n");
    printf("Forward and backward 3D complex inplace transform\n");
    printf("Configuration parameters:\n");
    printf(" N = {%d, %d, %d}\n", N[0], N[1], N[2]);
    printf(" H = {%d, %d, %d}\n", H[0], H[1], H[2]);

    printf("Allocate array for input data\n");
    x  = fftwf_malloc(sizeof(fftwf_complex)*N[0]*N[1]*N[2]);
    if (0 == x) goto failed;

    printf("Create FFTW plan for 3D double-precision forward transform\n");
    forward_plan = fftwf_plan_dft(3, N, x, x, FFTW_FORWARD, FFTW_ESTIMATE);
    if (0 == forward_plan) goto failed;

    printf("Create FFTW plan for 3D double-precision backward transform\n");
    backward_plan = fftwf_plan_dft(3, N, x, x, FFTW_BACKWARD, FFTW_ESTIMATE);
    if (0 == backward_plan) goto failed;

    printf("Initialize input for forward transform\n");
    init(x, N[0],N[1],N[2], H[0],H[1],H[2]);

    printf("Compute forward FFT\n");
    fftwf_execute(forward_plan);

    printf("Verify the result of forward FFT\n");
    status = verify(x, N[0],N[1],N[2], H[0],H[1],H[2]);
    if (0 != status) goto failed;

    printf("Initialize input for backward transform\n");
    init(x, N[0],N[1],N[2], -H[0],-H[1],-H[2]);

    printf("Compute backward transform\n");
    fftwf_execute(backward_plan);

    printf("Verify the result of backward FFT\n");
    status = verify(x, N[0],N[1],N[2], H[0],H[1],H[2]);
    if (0 != status) goto failed;

 cleanup:

    printf("Destroy FFTW plans\n");
    fftwf_destroy_plan(forward_plan);
    fftwf_destroy_plan(backward_plan);

    printf("Free data array\n");
    fftwf_free(x);

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

/* Initialize array x[N1][N2][N3][N4] with harmonic H */
static void init(fftwf_complex *x,
                 int N1,int N2,int N3,
                 int H1,int H2,int H3)
{
    float TWOPI = 6.2831853071795864769f, phase;
    int n1, n2, n3, index;

    /* Generalized strides for row-major addressing of x */
    int S1 = N2*N3, S2 = N3, S3 = 1;

    for (n1 = 0; n1 < N1; n1++)
    {
        for (n2 = 0; n2 < N2; n2++)
        {
            for (n3 = 0; n3 < N3; n3++)
            {
                phase  = moda(n1,H1,N1) / N1;
                phase += moda(n2,H2,N2) / N2;
                phase += moda(n3,H3,N3) / N3;
                index = n1*S1 + n2*S2 + n3*S3;
                x[index][0] = cosf( TWOPI * phase ) / (N1*N2*N3);
                x[index][1] = sinf( TWOPI * phase ) / (N1*N2*N3);
            }
        }
    }
}

/* Verify that x has unit peak at H */
static int verify(fftwf_complex *x,
                  int N1, int N2, int N3,
                  int H1, int H2, int H3)
{
    float err, errthr, maxerr;
    int n1, n2, n3, index;

    /* Generalized strides for row-major addressing of x */
    int S1 = N2*N3, S2 = N3, S3 = 1;

    /*
     * Note, this simple error bound doesn't take into account error of
     * input data
     */
    errthr = 5.0f * logf( (float)N1*N2*N3 ) / logf(2.0f) * FLT_EPSILON;
    printf(" Verify the result, errthr = %.3g\n", errthr);

    maxerr = 0;
    for (n1 = 0; n1 < N1; n1++)
    {
        for (n2 = 0; n2 < N2; n2++)
        {
            for (n3 = 0; n3 < N3; n3++)
            {
                float re_exp = 0.0, im_exp = 0.0, re_got, im_got;

                if ((n1-H1)%N1==0 && (n2-H2)%N2==0 && (n3-H3)%N3==0)
                {
                    re_exp = 1;
                }

                index = n1*S1 + n2*S2 + n3*S3;
                re_got = x[index][0];
                im_got = x[index][1];
                err  = fabsf(re_got - re_exp) + fabsf(im_got - im_exp);
                if (err > maxerr) maxerr = err;
                if (!(err < errthr))
                {
                    printf(" x[%i][%i][%i]: ",n1,n2,n3);
                    printf(" expected (%.7g,%.7g), ",re_exp,im_exp);
                    printf(" got (%.7g,%.7g), ",re_got,im_got);
                    printf(" err %.3g\n", err);
                    printf(" Verification FAILED\n");
                    return 1;
                }
            }
        }
    }
    printf(" Verified, maximum error was %.3g\n", maxerr);
    return 0;
}
