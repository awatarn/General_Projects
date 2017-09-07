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
!    vslGetStreamStateBrng  Example Program Text
!******************************************************************************/

#include <stdio.h>

#include "mkl_vsl.h"
#include "errcheck.inc"

#define SEED    7777777
#define N       1000

int main()
{
    VSLStreamStatePtr stream;
    unsigned int seed;
    int r[N], errcode;
    MKL_INT brngExp = VSL_BRNG_WH+127;
    int brngObt = 0;

    /***** Initialize seed *****/
    seed = SEED;

    /***** Initialize streams *****/
    errcode = vslNewStream  ( &stream, brngExp, (MKL_INT)seed );
    CheckVslError( errcode );

    /***** Get BRNG number *****/
    brngObt = vslGetStreamStateBrng ( stream );

    /***** Printing results *****/
    printf(" Sample of vslGetStreamStateBrng\n");
    printf(" -------------------------------\n\n");
    printf(" Parameters:\n");
    printf("    seed = %d\n",seed);
    printf("    brng = %d\n\n",brngExp);

    if(brngObt != brngExp) {
        printf(" Error: returned value %d is incorrect (expected %d)!\n", brngObt,brngExp);
        return 1;
    }
    else {
        printf(" Returned %d as expected\n",brngObt);
    }

    errcode = vslDeleteStream( &stream );
    CheckVslError( errcode );

    return 0;
}
