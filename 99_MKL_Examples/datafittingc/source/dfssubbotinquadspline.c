/*******************************************************************************
* Copyright 2010-2017 Intel Corporation All Rights Reserved.
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
!    Construction of Subbotin spline Example Program Text
!******************************************************************************/

#include <stdio.h>

#include "mkl.h"
#include "errcheck.inc"
#include "generatedata.inc"
#include "rescheck.inc"

#define N              6 // number of break points
#define NY             1 // number of functions
#define NBC            2 // number of boundary conditions

#define NIC            (N+1)                // number of Subbotin spline knots
#define NSCOEFF        NY*N*DF_PP_QUADRATIC // total number of spline
                                            // coefficients

#define LEFT_LIMIT  -2.0 // left limit of interpolation interval
#define RIGHT_LIMIT  2.0 // right limit of interpolation interval
#define FREQ         1.5

int main()
{
    DFTaskPtr task;                     // Data Fitting task descriptor
    MKL_INT sorder;
    MKL_INT stype;
    MKL_INT nx;                         // number of break points
    MKL_INT xhint;                      // additional info about break points
    MKL_INT ny;                         // number of functions
    MKL_INT yhint;                      // additional info about function
    MKL_INT nscoeff;                    // number of spline coefficients
    MKL_INT scoeffhint;                 // additional info about spline
                                        // coefficients
    MKL_INT bc_type;                    // boundary conditions type
    MKL_INT nbc;                        // number of boundary conditions
    MKL_INT ic_type;                    // inernal conditions type
    MKL_INT nic;                        // number of knots needed for Subbotin
                                        // spline construction

    float x[N];                         // array of break points
    float ic[NIC];                      // array of Subbotin spline knots
    float y[N*NY];                      // function values
    float bc[NBC] = { 1.0, -1.0 };      // array of boundary conditions
    float scoeff[NSCOEFF];              // array of spline coefficients

    float left = LEFT_LIMIT, right = RIGHT_LIMIT;
    float freq = FREQ;
    float spline_val[N], left_val[N], right_val[N];
    float left_der[N], right_der[N];

    int i, j, errcode = 0;
    int errnums = 0;

    /***** Initializing parameters for Data Fitting task *****/

    sorder = DF_PP_QUADRATIC;
    stype  = DF_PP_SUBBOTIN;

    /***** Parameters describing interpolation interval *****/
    nx        = N;
    xhint     = DF_NON_UNIFORM_PARTITION;

    /***** Parameters describing function *****/
    ny         = NY;
    yhint      = DF_NO_HINT;

    /***** Parameters describing spline coefficients storage *****/
    nscoeff    = NSCOEFF;
    scoeffhint = DF_NO_HINT;

    /***** Parameters describing boundary conditions type *****/
    bc_type    = DF_BC_1ST_LEFT_DER | DF_BC_1ST_RIGHT_DER;
    nbc        = NBC;

    /***** Parameters describing internal conditions type *****/
    /* No internal conditions are provided for Subbotin spline */
    ic_type    = DF_IC_Q_KNOT;
    nic        = NIC;

    /***** Generate uniformly distributed break points *****/
    errcode = sUniformRandSortedData( x, left, right, nx );
    CheckDfError(errcode);

    /***** Generate array of Subbotin spline knots *****/
    ic[0]  = x[0];
    ic[nx] = x[nx-1];

    for( i = 1; i < nx; i++ )
    {
        ic[i] = ( x[i] + x[i-1] ) / 2.0;
    }

    /***** Generate function y = sin(2 * Pi * freq * x) *****/
    freq = FREQ;
    errcode = sSinDataNotUniformGrid( y, x, freq, nx );
    CheckDfError(errcode);

    /***** Create Data Fitting task *****/
    errcode = dfsNewTask1D( &task, nx, x, xhint, ny, y, yhint );
    CheckDfError(errcode);

    /***** Edit task parameters for Subbotin spline coefficients
           computation *****/
    errcode = dfsEditPPSpline1D( task, sorder, stype, bc_type, bc,
                                 ic_type, ic, scoeff, scoeffhint );
    CheckDfError(errcode);

    /***** Construct Subbotin spline using STD method *****/
    errcode = dfsConstruct1D( task, DF_PP_SPLINE, DF_METHOD_STD );
    CheckDfError(errcode);

    /***** Delete Data Fitting task *****/
    errcode = dfDeleteTask( &task );
    CheckDfError(errcode);

    /***** Check computed coefficients *****/

    /***** Check Subbotin spline values in break points *****/
    errcode = sCheckSubbBreakPoints( nx, x, ny, y, scoeff,
                                     spline_val );
    if ( errcode < 0 ) errnums++;

    /***** Check spline values and 1st derivatives consistency
           in Subbotin spline knots *****/
    errcode = sCheckSubbQNodes( nx, x, nic, ic, ny, scoeff,
                                left_val, right_val );
    if ( errcode < 0 ) errnums++;

    errcode = sCheckQuadSubb1stDerConsistency( nic, x, ic, ny, scoeff,
                                           left_der, right_der );
    if ( errcode < 0 ) errnums++;

    /***** Check boundary conditions *****/
    errcode = sCheckQuadBC( nx, x, ny, scoeff, bc_type, bc );
    if ( errcode < 0 ) errnums++;

    /***** Print results *****/
    printf("Number of break points : %d\n", (int)nx);

    /***** Print given function *****/
    printf("\n  X           Y\n");

    for( j = 0; j < nx; j++ )
    {
        printf(" %+lf   %+lf\n", x[j], y[j]);
    }

    /***** Print array of Subbotin spline knots *****/
    printf("\n  Subbotin spline knots:\n");

    for( j = 0; j < nic; j++ )
    {
        printf(" %+lf\n", ic[j]);
    }

    printf("\nBoundary conditions (1st derivatives on both ends):\n");
    printf(" %+lf   %+lf\n", bc[0], bc[1]);

    /***** Print computed spline coefficients *****/

    printf("\nSpline coefficients for Y :\n");
    printf("    X^0           X^1           X^2       ");
    printf("    P(X[i])  ");
    printf("    P(Q[i])       P(Q[i+1]) ");
    printf("    P'(Q[i])      P'(Q[i+1])\n");

    for( j = 0; j < nx; j++ )
    {
        printf(" %+11.6f   %+11.6f   %+11.6f   %+11.6f   %+11.6f   %+11.6f",
                scoeff[j*sorder], scoeff[j*sorder+1], scoeff[j*sorder+2],
                spline_val[j], right_val[j], left_val[j]);
        printf("   %+11.6f   %+11.6f\n", right_der[j], left_der[j]);
    }

    /***** Print summary of the test *****/
    if (errnums != 0)
    {
        printf("\n\nError: Computed Subbotin spline coefficients");
        printf(" are incorrect\n");
        return 1;
    }
    else
    {
        printf("\n\nComputed Subbotin spline coefficients");
        printf(" are correct\n");
    }

    return 0;
}
