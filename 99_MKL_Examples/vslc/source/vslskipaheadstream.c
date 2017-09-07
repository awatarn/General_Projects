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
!    vslSkipaheadStream  Example Program Text
!******************************************************************************/

#include <stdio.h>

#include "mkl_vsl.h"
#include "errcheck.inc"

#define SEED    7777777
#define N       1000
#define S       10
#define NS      100


int main()
{
    VSLStreamStatePtr stream;
    VSLStreamStatePtr streamS[S];
    int r [N];
    int rS[N];
    int seed = SEED, i, j, k, err = 0, errcode;

    /****** Create main stream *********/
    errcode = vslNewStream  ( &stream,   VSL_BRNG_MCG31,  (MKL_INT)seed );
    CheckVslError( errcode );
    /* Create skipahead streams as copies of the main one */
    for(i=0;i<S;i++)
    {
        errcode = vslCopyStream( &streamS[i], stream );
        CheckVslError( errcode );
#if defined(_MSC_VER)
        errcode = vslSkipAheadStream( streamS[i], (__int64)(i*NS) );
#else
        errcode = vslSkipAheadStream( streamS[i], (long long)(i*NS) );
#endif
        CheckVslError( errcode );
    }

    /**** Generate random numbers for main stream  ****/
    errcode = viRngUniformBits( VSL_RNG_METHOD_UNIFORMBITS_STD, stream, N, (unsigned int *)(r) );
    CheckVslError( errcode );
    /* Generate random numbers for skipahead streams  */
    for(i=0;i<S;i++)
    {
        errcode = viRngUniformBits( VSL_RNG_METHOD_UNIFORMBITS_STD, streamS[i], NS, (unsigned int *)(&(rS[i*NS])) );
        CheckVslError( errcode );
    }

    /***** Compare results *****/
    for ( j=0, i=0; i<N; i++ )
    {
      if(r[i] != rS[i])
          err++;
    }

    /***** Printing results *****/
    printf(" Sample of vslSkipaheadStream\n");
    printf(" ----------------------------\n\n");
    printf(" Parameters:\n");
    printf("    seed   =   %d\n\n",seed);


    printf(" Results (first 10 of 1000):\n");
    printf(" ---------------------------\n");
    for(i=0;i<10;i++) {
        printf("r[%d]=0x%08X rS[%d]=0x%08X\n",i,r[i],i,rS[i]);
    }

    printf("\n");
    if(err) {
        printf("Error: %d values are incorrect!\n", err);
        return 1;
    }
    else {
        printf(" Results of ordinary and Skipahead streams are identical.\n");
    }

    errcode = vslDeleteStream( &stream );
    CheckVslError( errcode );
    for(i=0;i<S;i++)
    {
        errcode = vslDeleteStream( &streamS[i] );
        CheckVslError( errcode );
    }

    return 0;
}
