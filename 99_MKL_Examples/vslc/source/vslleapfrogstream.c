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
!    vslLeapfrogStream  Example Program Text
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
    VSLStreamStatePtr streamL[S];
    int r [N];
    int rL[N];
    int seed = SEED, i, j, k, err = 0, errcode;

    /****** Create main stream *********/
    errcode = vslNewStream  ( &stream,   VSL_BRNG_MCG31,  (MKL_INT)seed );
    CheckVslError( errcode );
    /* Create leapfrog streams as copies of the main one */
    for(i=0;i<S;i++)
    {
        errcode = vslCopyStream( &streamL[i], stream );
        CheckVslError( errcode );
        errcode = vslLeapfrogStream( streamL[i], (MKL_INT)i, S );
        CheckVslError( errcode );
    }

    /**** Generate random numbers for main stream  ****/
    errcode = viRngUniformBits( 0, stream, N, (unsigned int *)(r) );
    CheckVslError( errcode );
    /* Generate random numbers for leapfrog streams  */
    for(i=0;i<S;i++)
    {
        errcode = viRngUniformBits( VSL_RNG_METHOD_UNIFORMBITS_STD, streamL[i], NS, (unsigned int *)(&(rL[i*NS])) );
        CheckVslError( errcode );
    }

    /***** Compare results *****/
    for ( j=0, i=0; i<NS; i++ )
    {
       for ( k=0; k<S; k++ )
       {
      if(r[j++] != rL[k*NS+i])
          err++;
       }
    }

    /***** Printing results *****/
    printf(" Sample of vslLeapfrogStream\n");
    printf(" ---------------------------\n\n");
    printf(" Parameters:\n");
    printf("    seed   =   %d\n\n",seed);


    printf(" Results (first 10 of 1000):\n");
    printf(" ---------------------------\n");
    for(i=0;i<10;i++) {
        printf("r[%d]=0x%08X rL[%3d]=0x%08X\n",i,r[i],i*NS,rL[i*NS]);
    }

    printf("\n");
    if(err) {
        printf("Error: %d values are incorrect!\n", err);
        return 1;
    }
    else {
        printf(" Results of ordinary and Leapfrog streams are identical.\n");
    }

    errcode = vslDeleteStream( &stream );
    CheckVslError( errcode );
    for(i=0;i<S;i++)
    {
        errcode = vslDeleteStream( &streamL[i] );
        CheckVslError( errcode );
    }

    return 0;
}
