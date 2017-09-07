/*******************************************************************************
* Copyright 2004-2017 Intel Corporation All Rights Reserved.
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
*   Content : Intel(R) MKL PARDISO C example
*
********************************************************************************
*/
/* -------------------------------------------------------------------- */
/* Example program to show the use of the "matrix_check" routine */
/* on symmetric linear systems */
/* -------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "mkl_types.h"
#include "mkl_sparse_handle.h"

int main (void)
{
    /* Matrix data. */
    MKL_INT n = 8;
    MKL_INT ia[9] = { 1, 5, 8, 10, 12, 15, 17, 18, 19};
    MKL_INT ja[18] =
      { 1,    3,       6, 7,
           2, 3,    5,
              3,             8,
                 4,       7,
                 4,    6, 7,
                       6,    8,
                          7,
                             8
      };
    double a[18] =
      { 7.0,      1.0,           2.0, 7.0,
            -4.0, 8.0,      2.0,
                  1.0,                     5.0,
                       7.0,           9.0,
                       5.0,      1.0, 5.0,
                                -1.0,      5.0,
                                     11.0,
                                           5.0
      };

    sparse_checker_error_values check_err_val;
    sparse_struct pt;
    int error = 0;


    sparse_matrix_checker_init(&pt);
    pt.n = n;
    pt.csr_ia = ia;
    pt.csr_ja = ja;
    pt.indexing         = MKL_ONE_BASED;
    pt.matrix_structure = MKL_UPPER_TRIANGULAR;
    pt.print_style      = MKL_C_STYLE;
    pt.message_level    = MKL_PRINT;

    check_err_val = sparse_matrix_checker(&pt);

    printf("Matrix check details: (%d, %d, %d)\n", pt.check_result[0], pt.check_result[1], pt.check_result[2]);

    if ( check_err_val == MKL_SPARSE_CHECKER_NONTRIANGULAR) {
        printf("Matrix check result: MKL_SPARSE_CHECKER_NONTRIANGULAR\n");
        error = 0;
    }
    else {
        if ( check_err_val == MKL_SPARSE_CHECKER_SUCCESS) { printf("Matrix check result: MKL_SPARSE_CHECKER_SUCCESS\n"); }
        if ( check_err_val == MKL_SPARSE_CHECKER_NON_MONOTONIC) { printf("Matrix check result: MKL_SPARSE_CHECKER_NON_MONOTONIC\n"); }
        if ( check_err_val == MKL_SPARSE_CHECKER_OUT_OF_RANGE) { printf("Matrix check result: MKL_SPARSE_CHECKER_OUT_OF_RANGE\n"); }
        if ( check_err_val == MKL_SPARSE_CHECKER_NONORDERED) { printf("Matrix check result: MKL_SPARSE_CHECKER_NONORDERED\n"); }
        error = 1;
    }

    return error;
}
