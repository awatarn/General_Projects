/*******************************************************************************
* Copyright 2003-2017 Intel Corporation All Rights Reserved.
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
!  Content:
!    stream2file functions  Example Program Text
!******************************************************************************/

#include <stdio.h>
#include "mkl_vsl.h"
#include "errcheck.inc"

/* Quantity of random numbers to generate */
#define N 10

static float r_orig[N], r_load[N];

int main(void)
{
    VSLStreamStatePtr stream;
    int i, errcode;

    /* Create the original stream to be saved in a file */
    errcode = vslNewStream(&stream, VSL_BRNG_R250, 7777777);
    CheckVslError( errcode );

    /* Save original stream to a file */
    errcode = vslSaveStreamF(stream, "vslstream2file.dat");
    CheckVslError( errcode );

    /* Generate random numbers using original stream */
    errcode = vsRngUniform(0, stream, N, r_orig, 0.0f, 1.0f);
    CheckVslError( errcode );

    /* Delete original stream */
    errcode = vslDeleteStream(&stream);
    CheckVslError( errcode );

    /* Load stream that is saved in a file */
    errcode = vslLoadStreamF(&stream, "vslstream2file.dat");
    CheckVslError( errcode );

    /* Generate random numbers using the stream loaded from file */
    errcode = vsRngUniform(VSL_RNG_METHOD_UNIFORM_STD, stream, N, r_load, 0.0f, 1.0f);
    CheckVslError( errcode );

    /* Delete stream loaded from file */
    errcode = vslDeleteStream(&stream);
    CheckVslError( errcode );

    /* Compare random numbers from original and loaded stream.
       Must be identical */
    for ( i=0; i<N; i++ )
    {
        printf("r_orig[%d]=%f\tr_load[%d]=%f\n", i, r_orig[i], i, r_load[i]);
        if ( r_orig[i] != r_load[i] )
        {
            /* Here if results are not identical */
            printf("Error: Loaded stream differs from original stream.\n");
            return 1;
        }
    }

    /* Here if results are identical */
    printf("PASS: Loaded stream identical with original stream.\n");

    return 0;
}
