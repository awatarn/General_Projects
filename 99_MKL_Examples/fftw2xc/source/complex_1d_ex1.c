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
! Complex-to-complex 1D transform for double/single precision data inplace.
!
! Configuration parameters for Intel(R) MKL DFTI:
!           DFTI_FORWARD_DOMAIN = DFTI_COMPLEX (obligatory)
!           DFTI_PRECISION      = DFTI_DOUBLE/DFTI_SINGLE (obligatory)
!           DFTI_DIMENSION      = 1            (obligatory)
!           DFTI_LENGTHS        = n            (obligatory)
!           DFTI_PLACEMENT      = DFTI_INPLACE (default)
!           DFTI_FORWARD_SCALE  = 1.0          (default)
!           DFTI_BACKWARD_SCALE = 1.0/n        (default=1.0)
!
! Other default configuration parameters are in the mkl_dfti.h interface file
!****************************************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "fftw.h"
#include "mkl_dfti_examples.h"

int main(void)  /* COMPLEX_1D_EX1 */
{
    /*
    **  DFT input parameters
    */
    int n = 4;

    fftw_plan my_plan;
    fftw_complex* x_in;
    fftw_complex* x_exp;

    TYPE_PRECISION Scale;
    TYPE_PRECISION maxerr;
    TYPE_PRECISION eps = EPS;

    if (LEGEND_PRINT) {
        printf("\n\n COMPLEX_1D_EX1\n");
        printf(" Forward-Backward 1D complex transform for double/single precision data\n\n");
        printf(" Configuration parameters:\n\n");
        printf(" DFTI_FORWARD_DOMAIN = DFTI_COMPLEX\n");
        printf(" DFTI_PRECISION      = DFTI_DOUBLE/DFTI_SINGLE\n");
        printf(" DFTI_DIMENSION      = 1\n");
        printf(" DFTI_LENGTHS        = %d\n", n);
        printf(" DFTI_PLACEMENT      = DFTI_INPLACE\n");
        printf(" DFTI_FORWARD_SCALE  = 1.0\n");
        printf(" DFTI_BACKWARD_SCALE = 1.0/n\n\n");
    }

    /*
    **  Allocate array for input and expected data
    */
    x_in = (fftw_complex*)fftw_malloc(2*n*sizeof(TYPE_PRECISION));
    x_exp = (fftw_complex*)fftw_malloc(2*n*sizeof(TYPE_PRECISION));

    /*
    **  Initialize x_in and copy to expected x_exp
    */
#ifdef MKL_DOUBLE
    init_input_and_expected_vectors_z(x_in, x_exp, (long)n);
#else
    init_input_and_expected_vectors_c(x_in, x_exp, (long)n);
#endif

    /*
    **  Create FFTW plan for 1D double/single precision forward transform
    */
    my_plan = fftw_create_plan(n, FFTW_FORWARD, FFTW_ESTIMATE | FFTW_IN_PLACE);
    DIE_UNLESS(my_plan);

    /*
    **  Compute DFT
    */
    fftw_one(my_plan, x_in, NULL);

    /*
    **  Destroy FFTW plan
    */
    fftw_destroy_plan(my_plan);

    /*
    **  Set Scale number for Backward transform
    */
    Scale = 1.0/(TYPE_PRECISION)n;

    /*
    **  Create FFTW plan for 1D double/single precision backward transform
    */
    my_plan = fftw_create_plan(n, FFTW_BACKWARD, FFTW_ESTIMATE | FFTW_IN_PLACE);
    DIE_UNLESS(my_plan);

    /*
    **  Compute DFT
    */
    fftw_one(my_plan, x_in, NULL);

    /*
    **  Destroy FFTW plan
    */
    fftw_destroy_plan(my_plan);

    /*
    **  Result scaling
    */
    scaling(x_in, Scale, n);

    /*
    **  Check result
    */
#ifdef MKL_DOUBLE
    maxerr = check_result_z(x_in, x_exp, n);
#else
    maxerr = check_result_c(x_in, x_exp, n);
#endif

    /*
    **  Free arrays for input and expected data
    */
    fftw_free(x_in);
    fftw_free(x_exp);

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
