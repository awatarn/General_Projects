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
!    stream2memory functions  Example Program Text
!******************************************************************************/

#include <stdio.h>
#include "mkl_vsl.h"
#include "errcheck.inc"

/* Quantity of random numbers to generate */
#define DN 101

#define N   10

#define SEED       7777777
#define BRNG       VSL_BRNG_SFMT19937
#define METHOD     VSL_RNG_METHOD_GAUSSIAN_BOXMULLER2

int main(void)
{
    double rd_orig[DN], rd_load[DN];
    int size;
    char *membuf;

    VSLStreamStatePtr stream_orig, stream_load;
    int i, errcode;

    /* Create the original stream to be saved in a file */
    errcode = vslNewStream( &stream_orig, BRNG , SEED );
    CheckVslError( errcode );

    /* Generate random numbers using original stream */
    errcode = vdRngGaussian( METHOD, stream_orig, DN, rd_orig, 0.0, 1.0);
    CheckVslError( errcode );

    /* Generation of Gaussian random numbers */
    /* Compute memory size necessary for random stream descriptive data */
    size = vslGetStreamSize( stream_orig );
    membuf = (char*)malloc( sizeof(char*) * size );
    if ( membuf == 0 )
    {
        printf("memeory allocation error\n");
        return 1;
    }
    errcode = vslSaveStreamM( stream_orig, membuf );
    CheckVslError( errcode );

    /* Generate random numbers using original stream */
    errcode = vdRngGaussian( METHOD, stream_orig, DN, rd_orig, 0.0, 1.0);
    CheckVslError( errcode );

    /* Load stream from the memeory */
    errcode = vslLoadStreamM( &stream_load, membuf );
    CheckVslError( errcode );

    /* Generate random numbers using loaded stream */
    errcode = vdRngGaussian( METHOD, stream_load, DN, rd_load, 0.0, 1.0);
    CheckVslError( errcode );

    /* Compare random numbers from original and loaded stream.
       Must be identical */
    printf("Gaussian numbers:\n");
    for ( i=0; i<N; i++ )
    {
        printf("rd_orig[%d]=%f\trd_load[%d]=%f\n", i, rd_orig[i], i, rd_load[i]);
    }

    for ( i=0; i<DN; i++ )
    {
        if ( rd_orig[i] != rd_load[i] )
        {
            /* Here if results are not identical */
            printf("Error: Loaded stream differs from original stream.\n");
            return 1;
        }
    }

    free( membuf );

    /* Delete stream loaded from memory */
    errcode = vslDeleteStream(&stream_load);
    CheckVslError( errcode );

    /* Delete original stream */
    errcode = vslDeleteStream(&stream_orig);
    CheckVslError( errcode );

    /* Here if results are identical */
    printf("PASS: Loaded stream identical with original stream.\n");

    return 0;
}
