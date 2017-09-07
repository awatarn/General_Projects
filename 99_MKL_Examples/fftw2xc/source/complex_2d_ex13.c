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
! Forward-Backward 2D complex transform for double/single precision data inplace.
!
! Configuration parameters:
!           DFTI_FORWARD_DOMAIN = DFTI_COMPLEX  (obligatory)
!           DFTI_PRECISION      = DFTI_DOUBLE/DFTI_SINGLE (obligatory)
!           DFTI_DIMENSION      = 2             (obligatory)
!           DFTI_LENGTHS        = {m,n}         (obligatory)
!           DFTI_PLACEMENT      = DFTI_INPLACE  (default)
!           DFTI_INPUT_STRIDES  = {0, N_MAX, 1} (default={0,n,1})
!           DFTI_FORWARD_SCALE  = 1.0           (default)
!           DFTI_BACKWARD_SCALE = 1.0/(m*n)     (default=1.0)
!
! Other default configuration parameters are in the mkl_dfti.h interface file
!****************************************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "fftw_threads.h"
#include "mkl_dfti_examples.h"

int main(void)  /* COMPLEX_2D_EX1 */
{
    /*
    **  DFT input parameters
    */
    int m, n;

    fftwnd_plan my_plan;
    fftw_complex x_in[M_MAX][N_MAX];
    fftw_complex x_exp[M_MAX][N_MAX];

    TYPE_PRECISION Scale;
    TYPE_PRECISION maxerr;
    TYPE_PRECISION eps = EPS;

    /*
    ** Put transform parameters
    */
    m = M_MAX;
    n = N_MAX;

    if (LEGEND_PRINT) {
        printf("\n\n COMPLEX_2D_EX1\n");
        printf(" Forward-Backward 2D complex transform for double/single precision data\n\n");
        printf(" Configuration parameters:\n\n");
        printf(" DFTI_FORWARD_DOMAIN = DFTI_COMPLEX\n");
        printf(" DFTI_PRECISION      = DFTI_DOUBLE/DFTI_SINGLE\n");
        printf(" DFTI_DIMENSION      = 2\n");
        printf(" DFTI_LENGTHS        = {%d,%d}\n", m, n);
        printf(" DFTI_PLACEMENT      = DFTI_INPLACE\n");
        printf(" DFTI_INPUT_STRIDES  = {0,%d,1}\n", N_MAX);
        printf(" DFTI_FORWARD_SCALE  = 1.0\n");
        printf(" DFTI_BACKWARD_SCALE = 1.0/(m*n)\n\n");
    }

    /*
    **  Put input data and expected result
    */
#ifdef MKL_DOUBLE
    zero_init_z(x_in,  M_MAX*N_MAX);
    init_multiple_columns_z(x_in, m, n, 0, N_MAX);
    cblas_zcopy(M_MAX*N_MAX, x_in, 1, x_exp, 1);
#else
    zero_init_c(x_in,  M_MAX*N_MAX);
    init_multiple_columns_c(x_in, m, n, 0, N_MAX);
    cblas_ccopy(M_MAX*N_MAX, x_in, 1, x_exp, 1);
#endif

    fftw_threads_init();

    /*
    **  Create FFTW plan for 2D double/single precision forward transform
    */
    my_plan = fftw2d_create_plan(m, n, FFTW_FORWARD, FFTW_ESTIMATE | FFTW_IN_PLACE);
    DIE_UNLESS(my_plan);

    /*
    **  Compute DFT
    */
    fftwnd_threads_one(1, my_plan, &x_in[0][0], NULL);

    /*
    **  Destroy FFTW plan
    */
    fftwnd_destroy_plan(my_plan);

    /*
    **  Set Scale number for Backward transform
    */
    Scale = 1.0/(TYPE_PRECISION)(m*n);

    /*
    **  Create FFTW plan for 2D double/single precision backward transform
    */
    my_plan = fftw2d_create_plan(m, n, FFTW_BACKWARD, FFTW_ESTIMATE | FFTW_IN_PLACE);
    DIE_UNLESS(my_plan);

    /*
    **  Compute DFT
    */
    fftwnd_threads_one(1, my_plan, &x_in[0][0], NULL);

    /*
    **  Destroy FFTW plan
    */
    fftwnd_destroy_plan(my_plan);

    /*
    **  Result scaling
    */
    scaling(x_in, Scale, m*n);

    /*
    **  Check result
    */
#ifdef MKL_DOUBLE
    maxerr = check_result_z(x_in, x_exp, M_MAX*N_MAX);
#else
    maxerr = check_result_c(x_in, x_exp, M_MAX*N_MAX);
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
