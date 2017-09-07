/*******************************************************************************
* Copyright 2006-2017 Intel Corporation All Rights Reserved.
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
!          Intel(R) MKL DFTI implementation through FFTW interface (via wrappers) example
!          program (C-interface)
!
! Forward-Backward 3D complex transform for double/single precision data inplace.
!
! Configuration parameters:
!           DFTI_FORWARD_DOMAIN = DFTI_COMPLEX (obligatory)
!           DFTI_PRECISION      = DFTI_DOUBLE/DFTI_SINGLE (obligatory)
!           DFTI_DIMENSION      = 3            (obligatory)
!           DFTI_LENGTHS        = {m,n,k}      (obligatory)
!           DFTI_PLACEMENT      = DFTI_INPLACE (default)
!           DFTI_INPUT_STRIDES  = {first_index, step_in_m, step_in_n, step_in_k}
!                                              (default={0,n*k,k,1})
!           DFTI_FORWARD_SCALE  = 1.0          (default)
!           DFTI_BACKWARD_SCALE = 1.0/(m*n*k)  (default=1.0)
!           DFTI_NUMBER_OF_TRANSFORMS = 1      (default)
!
! Other default configuration parameters are in the mkl_dfti.h interface file
!****************************************************************************/

#include <stdio.h>
#include <math.h>
#include "fftw_threads.h"
#include "mkl_dfti_examples.h"

int main(void)  /* COMPLEX_3D_EX13 */
{
    /*
    **  DFT input parameters
    */
    int m = M_MAX;
    int n = N_MAX;
    int k = K_MAX;
    int first_index_in = 0;
    int step_in_m = 1;
    int step_in_n = 1;
    int step_in_k = 1;
    long strides_in[4];

    fftwnd_plan my_plan;
    fftw_complex x_in[M_MAX][N_MAX][K_MAX];
    fftw_complex x_exp[M_MAX][N_MAX][K_MAX];

    TYPE_PRECISION Scale;
    TYPE_PRECISION maxerr;
    TYPE_PRECISION eps = EPS;

    /*
    ** Put transform parameters
    */
    strides_in[0] = (long)first_index_in;
    strides_in[1] = (long)step_in_m * N_MAX*K_MAX;
    strides_in[2] = (long)step_in_n * K_MAX;
    strides_in[3] = (long)step_in_k * 1;

    if (LEGEND_PRINT) {
        printf("\n\n COMPLEX_3D_EX13\n");
        printf(" Forward-Backward 3D complex transform for double/single precision data\n\n");
        printf(" Configuration parameters:\n\n");
        printf(" DFTI_FORWARD_DOMAIN = DFTI_COMPLEX\n");
        printf(" DFTI_PRECISION      = DFTI_DOUBLE/DFTI_SINGLE\n");
        printf(" DFTI_DIMENSION      = 3\n");
        printf(" DFTI_LENGTHS        = {%d,%d,%d}\n", m, n, k);
        printf(" DFTI_PLACEMENT      = DFTI_INPLACE\n");
        printf(" DFTI_INPUT_STRIDES  = {%ld, %ld, %ld, %ld}\n",
                strides_in[0], strides_in[1], strides_in[2], strides_in[3]);
        printf(" DFTI_FORWARD_SCALE  = 1.0\n");
        printf(" DFTI_BACKWARD_SCALE = 1.0/(m*n*k)\n\n");
    }

    /*
    **  Put input data and expected result
    */
#ifdef MKL_DOUBLE
    zero_init_z(x_in, M_MAX*N_MAX*K_MAX);
    init_3d_columns_z(x_in, m, n, k, strides_in);
    cblas_zcopy(M_MAX*N_MAX*K_MAX, x_in, 1, x_exp, 1);
#else
    zero_init_c(x_in, M_MAX*N_MAX*K_MAX);
    init_3d_columns_c(x_in, m, n, k, strides_in);
    cblas_ccopy(M_MAX*N_MAX*K_MAX, x_in, 1, x_exp, 1);
#endif

    fftw_threads_init();

    /*
    **  Create FFTW plan for 3D double/single precision forward transform
    */
    my_plan = fftw3d_create_plan(m, n, k, FFTW_FORWARD, FFTW_ESTIMATE | FFTW_IN_PLACE);
    DIE_UNLESS(my_plan);

    /*
    **  Compute DFT
    */
    fftwnd_threads_one(1, my_plan, &x_in[0][0][0], &x_in[0][0][0]);

    /*
    **  Destroy FFTW plan
    */
    fftwnd_destroy_plan(my_plan);

    /*
    **  Set Scale number for Backward transform
    */
    Scale = 1.0/(double)(m*n*k);

    /*
    **  Create FFTW plan for 3D double/single precision backward transform
    */
    my_plan = fftw3d_create_plan(m, n, k, FFTW_BACKWARD, FFTW_ESTIMATE | FFTW_IN_PLACE);
    DIE_UNLESS(my_plan);

    /*
    **  Compute DFT
    */
    fftwnd_threads_one(1, my_plan, &x_in[0][0][0], &x_in[0][0][0]);

    /*
    **  Destroy FFTW plan
    */
    fftwnd_destroy_plan(my_plan);

    /*
    **  Result scaling
    */
    scaling(x_in, Scale, M_MAX*N_MAX*K_MAX);

    /*
    **  Check result
    */
#ifdef MKL_DOUBLE
    maxerr = check_result_z(x_in, x_exp, M_MAX*N_MAX*K_MAX);
#else
    maxerr = check_result_c(x_in, x_exp, M_MAX*N_MAX*K_MAX);
#endif

    if (ACCURACY_PRINT)
        printf("\n Accuracy = %g\n\n", maxerr);

    if (maxerr < eps) {
        printf(" TEST PASSED\n");
    } else {
        printf(" TEST FAILED\n");
        return 1;
    }

    printf(" END OF TEST\n");

    return 0;
}
