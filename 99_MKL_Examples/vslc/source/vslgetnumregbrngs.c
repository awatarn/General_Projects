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
!    vslGetNumRegBrngs  Example Program Text
!******************************************************************************/

#include <stdio.h>

#include "mkl_vsl.h"
#include "errcheck.inc"

#define BRNGS   16

int main()
{
    int brngsExp = BRNGS;
    int brngsObt = 0;

    /***** Get number of registered BRNGs *****/
    brngsObt = vslGetNumRegBrngs ();

    /***** Printing results *****/
    printf(" Sample of vslGetNumRegBrngs\n");
    printf(" -------------------------------\n\n");

    if(brngsObt != brngsExp) {
        printf(" Error: returned value %d is incorrect (expected %d)!\n", brngsObt,brngsExp);
        return 1;
    }
    else {
        printf(" Returned %d as expected\n",brngsObt);
    }

    return 0;
}
