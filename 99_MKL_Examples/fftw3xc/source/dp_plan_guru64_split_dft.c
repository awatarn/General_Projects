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
!       Example of using fftw_plan_guru64_split_dft function.
!
!****************************************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <float.h>
#include "fftw3.h"

/* Define format to printf ptrdiff_t values used by guru64 interfaces */
#if defined(_WIN32)
#define LI "%Ii" /* on Windows */
#else
#define LI "%zi" /* Otherwise */
#endif

static void init_i(double *re, double *im,
                   fftw_iodim64 *ndims, fftw_iodim64 *vdims,
                   ptrdiff_t H1, ptrdiff_t H2, ptrdiff_t H3);
static int verify_o(double *re, double *im,
                    fftw_iodim64 *ndims, fftw_iodim64 *vdims,
                    ptrdiff_t H1, ptrdiff_t H2, ptrdiff_t H3);
static ptrdiff_t  isize2(int n, fftw_iodim64 *ndims, int m, fftw_iodim64 *vdims);
static void print_dims(int n,fftw_iodim64 *dims);

int main(void)
{
    /*
     * In this example we show how to compute multiple
     * three-dimensional split-complex in-place FFTs by one call of
     * FFTW.
     */

    /* Sizes of 3D transform and the number of them */
    ptrdiff_t N1 = 5, N2 = 6, N3 = 7;
    ptrdiff_t M = 8;

    /* Arbitrary harmonic used to verify FFT */
    ptrdiff_t H1 = -1, H2 = -2, H3 = -3;

    /* One FFTW plan handles both forward and backward transforms. */
    fftw_plan plan = 0;

    /* FFTW iodims used to define the data layout */
    fftw_iodim64 dim[3], M_dim;

    /* Pointers to input/output data */
    double *x_re = 0, *x_im = 0;

    /* Execution status */
    int status = 0;


    printf("Example dp_plan_guru64_split_dft\n");
    printf("Forward and backward multiple 3D complex in-place FFT\n");
    printf("Configuration parameters:\n");
    printf(" N  = {"LI", "LI", "LI"}\n", N1, N2, N3);
    printf(" M  = "LI"\n", M);
    printf(" H  = {"LI", "LI", "LI"}\n", H1, H2, H3);

    printf("Define iodims for forward transform\n");
    dim[0].n    = N1;
    dim[1].n    = N2;
    dim[2].n    = N3;
    M_dim.n     = M;

    /*
     * Input/output array x: element x(m,n1,n2,n3) is located at
     * x_re[m*M_dim.is+n1*dim[0].is+n2*dim[1].is+n3*dim[2].is] and
     * x_im[m*M_dim.is+n1*dim[0].is+n2*dim[1].is+n3*dim[2].is].
     *
     * Input strides are equal to respective output strides for
     * simplicity. We define them as if x_re and x_im were declared
     * x_xx[M][N1][N2][N3], but other layouts are possible.
     */
    dim[2].is = dim[2].os  = 1;
    dim[1].is = dim[1].os  = N3;
    dim[0].is = dim[0].os  = N2*N3;
    M_dim.is  = M_dim.os   = N1*N2*N3;

    printf(" dim   = ");  print_dims(3,dim); printf("\n");
    printf(" M_dim = ");  print_dims(1,&M_dim); printf("\n");

    printf("Allocate x_re and x_im, "LI" elements each\n", isize2(3,dim,1,&M_dim) );
    x_re  = fftw_malloc(sizeof(double) * isize2(3,dim,1,&M_dim) );
    x_im  = fftw_malloc(sizeof(double) * isize2(3,dim,1,&M_dim) );
    if (0 == x_re || 0 == x_im) goto failed;

    printf("Create FFTW plan for forward/backward transform\n");
    plan = fftw_plan_guru64_split_dft(3, dim, 1, &M_dim, x_re, x_im, x_re, x_im,
                                     FFTW_ESTIMATE);
    if (0 == plan) goto failed;

    printf("Initialize input for forward transform\n");
    init_i(x_re, x_im, dim, &M_dim, H1,H2,H3);

    printf("Compute forward FFT\n");
    fftw_execute(plan);

    printf("Verify the result of forward FFT\n");
    status = verify_o(x_re, x_im, dim, &M_dim, H1,H2,H3);
    if (0 != status) goto failed;

    printf("Initialize input for backward transform\n");
    init_i(x_re, x_im, dim, &M_dim, -H1,-H2,-H3);

    printf("Compute backward transform by using new-array execute function\n");
    fftw_execute_split_dft(plan, x_im, x_re, x_im, x_re);

    printf("Verify the result of backward FFT\n");
    status = verify_o(x_re, x_im, dim, &M_dim, H1,H2,H3);
    if (0 != status) goto failed;

 cleanup:

    printf("Destroy FFTW plans\n");
    fftw_destroy_plan(plan);

    printf("Free data arrays\n");
    fftw_free(x_re);
    fftw_free(x_im);

    printf("TEST %s\n",0==status ? "PASSED" : "FAILED");
    return status;

 failed:
    printf(" ERROR\n");
    status = 1;
    goto cleanup;
}

/* Compute the input size, in elements */
static ptrdiff_t  isize2(int n, fftw_iodim64 *ndims, int m, fftw_iodim64 *vdims)
{
    int i;
    ptrdiff_t res = 1;
    for (i = 0; i < n; ++i)
    {
        if (res < ndims[i].n*ndims[i].is) res = ndims[i].n*ndims[i].is;
    }
    for (i = 0; i < m; ++i)
    {
        if (res < vdims[i].n*vdims[i].is) res = vdims[i].n*vdims[i].is;
    }
    return res;
}

/* Print iodims */
static void print_dims(int n, fftw_iodim64 *dims)
{
    int i;
    printf(LI":"LI":"LI, dims[0].n, dims[0].is, dims[0].os);
    for (i=1; i<n; ++i)
    {
        printf("x"LI":"LI":"LI, dims[i].n, dims[i].is, dims[i].os);
    }
}

/* Compute (K*L)%M accurately, useful when sizeof(ptrdiff_t)==sizeof(int) */
static double moda(ptrdiff_t K, ptrdiff_t L, ptrdiff_t M)
{
    return (double)(((long long)K * L) % M);
}

/* Initialize arrays x with harmonic H, using *.is for indexing */
static void init_i(double *re, double *im,
                   fftw_iodim64 *ndims, fftw_iodim64 *vdims,
                   ptrdiff_t H1,ptrdiff_t H2,ptrdiff_t H3)
{
    double TWOPI = 6.2831853071795864769, phase;
    ptrdiff_t n1, n2, n3, m, N1, N2, N3, M, S1, S2, S3, SM, index;

    N1 = ndims[0].n; S1 = ndims[0].is;
    N2 = ndims[1].n; S2 = ndims[1].is;
    N3 = ndims[2].n; S3 = ndims[2].is;
    M  = vdims[0].n; SM = vdims[0].is;
    for (m = 0; m < M; m++)
    {
        for (n1 = 0; n1 < N1; n1++)
        {
            for (n2 = 0; n2 < N2; n2++)
            {
                for (n3 = 0; n3 < N3; n3++)
                {
                    phase  = moda(n1,H1,N1) / N1;
                    phase += moda(n2,H2,N2) / N2;
                    phase += moda(n3,H3,N3) / N3;
                    index = n1*S1 + n2*S2 + n3*S3 + m*SM;
                    re[index] = cos( TWOPI * phase ) / (N1*N2*N3);
                    im[index] = sin( TWOPI * phase ) / (N1*N2*N3);
                }
            }
        }
    }
}

/* Verify that x has unit peak at H, using *.os for indexing */
static int verify_o(double *re, double *im,
                    fftw_iodim64 *ndims, fftw_iodim64 *vdims,
                    ptrdiff_t H1,ptrdiff_t H2,ptrdiff_t H3)
{
    double err, errthr, maxerr;
    ptrdiff_t n1, n2, n3, m, N1, N2, N3, M, S1, S2, S3, SM, index;

    N1 = ndims[0].n; S1 = ndims[0].os;
    N2 = ndims[1].n; S2 = ndims[1].os;
    N3 = ndims[2].n; S3 = ndims[2].os;
    M  = vdims[0].n; SM = vdims[0].os;

    /*
     * Note, this simple error bound doesn't take into account error of
     * input data
     */
    errthr = 5.0 * log( (double)N1*N2*N3 ) / log(2.0) * DBL_EPSILON;
    printf(" Verify the result, errthr = %.3lg\n", errthr);

    maxerr = 0;
    for (m = 0; m < M; m++)
    {
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

                    index = n1*S1 + n2*S2 + n3*S3 + m*SM;
                    re_got = re[index];
                    im_got = im[index];
                    err  = fabs(re_got - re_exp) + fabs(im_got - im_exp);
                    if (err > maxerr) maxerr = err;
                    if (!(err < errthr))
                    {
                        printf(" x(n1="LI",n2="LI",n3="LI",m="LI"): ",n1,n2,n3,m);
                        printf(" expected (%.17lg,%.17lg), ",re_exp,im_exp);
                        printf(" got (%.17lg,%.17lg), ",re_got,im_got);
                        printf(" err %.3lg\n", err);
                        printf(" Verification FAILED\n");
                        return 1;
                    }
                }
            }
        }
    }
    printf(" Verified, maximum error was %.3lg\n", maxerr);
    return 0;
}
