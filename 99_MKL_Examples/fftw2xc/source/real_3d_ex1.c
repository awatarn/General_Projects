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
! Real 3D transform for double/single precision data inplace.
!
! Configuration parameters for Intel(R) MKL DFTI:
!           DFTI_FORWARD_DOMAIN = DFTI_REAL    (obligatory)
!           DFTI_PRECISION      = DFTI_DOUBLE/DFTI_SINGLE (obligatory)
!           DFTI_DIMENSION      = 3            (obligatory)
!           DFTI_LENGTHS        = {m,n,k}      (obligatory)
!           DFTI_PLACEMENT      = DFTI_INPLACE (default)
!           DFTI_FORWARD_SCALE  = 1.0          (default)
!           DFTI_BACKWARD_SCALE = 1.0/(m*n*k)  (default=1.0)
!
! Other default configuration parameters are in the mkl_dfti.h interface file
!****************************************************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "rfftw.h"
#include "mkl_dfti_examples.h"

int main(void)  /* REAL_3D_EX1 */
{
    /*
    **  DFT input parameters
    */
    int m = 5;
    int n = 4;
    int k = 3;
    int nn;

    rfftwnd_plan my_plan;
    fftw_real* x_in;
    fftw_real* x_exp;

    TYPE_PRECISION Scale;
    TYPE_PRECISION maxerr;
    TYPE_PRECISION eps = EPS;

    if (LEGEND_PRINT) {
        printf("\n\n REAL_3D_EX1\n");
        printf(" Forward-Backward 3D real transform for double/single precision data\n\n");
        printf(" Configuration parameters:\n\n");
        printf(" DFTI_FORWARD_DOMAIN = DFTI_REAL\n");
        printf(" DFTI_PRECISION      = DFTI_DOUBLE/DFTI_SINGLE\n");
        printf(" DFTI_DIMENSION      = 3\n");
        printf(" DFTI_LENGTHS        = {%d,%d,%d}\n", m, n, k);
        printf(" DFTI_PACKED_FORMAT  = DFTI_CCE_FORMAT\n");
        printf(" DFTI_PLACEMENT      = DFTI_INPLACE\n");
        printf(" DFTI_FORWARD_SCALE  = 1.0\n");
        printf(" DFTI_BACKWARD_SCALE = 1.0/(m*n*k)\n\n");
    }

    /*
    **  Allocate array for input and expected data
    */
    nn = 2*(k/2+1)*m*n;

    x_in = (fftw_real*)fftw_malloc(nn*sizeof(TYPE_PRECISION));
    x_exp = (fftw_real*)fftw_malloc(nn*sizeof(TYPE_PRECISION));

    /*
    **  Initialize x_in and copy to expected x_exp
    */
#ifdef MKL_DOUBLE
    zero_init_d(x_in, nn);
    init_real_vectors_d(x_in, nn);
    cblas_dcopy(nn, x_in, 1, x_exp, 1);
#else
    zero_init_s(x_in, nn);
    init_real_vectors_s(x_in, nn);
    cblas_scopy(nn, x_in, 1, x_exp, 1);
#endif

    /*
    **  Create FFTW plan for 3D double/single precision forward transform
    */
    my_plan = rfftw3d_create_plan(m, n, k, FFTW_REAL_TO_COMPLEX, FFTW_ESTIMATE | FFTW_IN_PLACE);
    DIE_UNLESS(my_plan);

    /*
    **  Compute DFT
    */
    rfftwnd_one_real_to_complex(my_plan, x_in, NULL);

    /*
    **  Destroy FFTW plan
    */
    rfftwnd_destroy_plan(my_plan);

    /*
    **  Set Scale number for Backward transform
    */
    Scale = 1.0/(TYPE_PRECISION)(n*m*k);

    /*
    **  Create FFTW plan for 3D double/single precision backward transform
    */
    my_plan = rfftw3d_create_plan(m, n, k, FFTW_COMPLEX_TO_REAL, FFTW_ESTIMATE | FFTW_IN_PLACE);
    DIE_UNLESS(my_plan);

    /*
    **  Compute DFT
    */
    rfftwnd_one_complex_to_real(my_plan, (fftw_complex*)x_in, NULL);

    /*
    **  Destroy FFTW plan
    */
    rfftwnd_destroy_plan(my_plan);

    /*
    **  Result scaling
    */
    scaling_3d_r(x_in, Scale, m, n, k);

    /*
    **  Check result
    */
    maxerr = check_result_3d_r(x_in, x_exp, m, n, k);

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
