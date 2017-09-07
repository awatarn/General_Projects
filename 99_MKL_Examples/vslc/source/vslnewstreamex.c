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
!    vslNewStreamEx  Example Program Text
!******************************************************************************/

#include <stdio.h>

#include "mkl_vsl.h"
#include "errcheck.inc"

#define SEED    7777777
#define N       1000

int main()
{
    VSLStreamStatePtr stream;
    VSLStreamStatePtr streamEx;
    unsigned int seed;
    unsigned int seedEx[6];
    int r[N];
    int rEx[N];
    int i, err = 0, errcode;

    /***** Initialize seeds *****/
    seed = SEED;
    seedEx[0] = SEED;
    for(i = 1; i < 6; i++){
      seedEx[i] = 1;
    }

    /***** Initialize streams *****/
    errcode = vslNewStream  ( &stream,   VSL_BRNG_MRG32K3A,    (MKL_INT)seed );
    CheckVslError( errcode );
    errcode = vslNewStreamEx( &streamEx, VSL_BRNG_MRG32K3A, 6,          seedEx );
    CheckVslError( errcode );

    /***** Call RNGs *****/
    errcode = viRngUniformBits( VSL_RNG_METHOD_UNIFORMBITS_STD, stream,   N, (unsigned int *)(r) );
    CheckVslError( errcode );
    errcode = viRngUniformBits( VSL_RNG_METHOD_UNIFORMBITS_STD, streamEx, N, (unsigned int *)(rEx) );
    CheckVslError( errcode );

    /***** Compare results *****/
    for(i = 0; i < N; i++){
      if(r[i] != rEx[i])
          err++;
    }

    /***** Printing results *****/
    printf(" Sample of vslNewStreamEx\n");
    printf(" ------------------------\n\n");
    printf(" Parameters:\n");
    printf("    seed   =   %d\n",seed);
    printf("    seedEx = { %d %d %d %d %d %d }\n\n",
      seedEx[0],seedEx[1],seedEx[2],
      seedEx[3],seedEx[4],seedEx[5]);


    printf(" Results (first 10 of 1000):\n");
    printf(" ---------------------------\n");
    for(i=0;i<10;i++) {
        printf("r[%d]=0x%08X rEx[%d]=0x%08X\n",i,r[i],i,rEx[i]);
    }

    printf("\n");
    if(err) {
        printf("Error: %d values are incorrect!\n", err);
        return 1;
    }
    else {
        printf(" Results of ordinary and extended NewStream functions are identical.\n");
    }

    errcode = vslDeleteStream( &stream );
    CheckVslError( errcode );
    errcode = vslDeleteStream( &streamEx );
    CheckVslError( errcode );

    return 0;
}
