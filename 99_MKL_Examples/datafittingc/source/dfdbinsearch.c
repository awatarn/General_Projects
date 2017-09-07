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
!    Cells search in blocks Example Program Text
!******************************************************************************/

#include <stdio.h>

#include "mkl.h"
#include "errcheck.inc"
#include "generatedata.inc"
#include "rescheck.inc"

#define N                     10    // number of breakpoints
#define NSITE_BLOCK           15    // number of sites for cell search in block
#define NBLOCKS                3    // number of blocks
#define LDATA                  5    // length of the datahint array

#define NSITE (NBLOCKS*NSITE_BLOCK) // total number of sites for cell search

#define LEFT_LIMIT         -10.0    // left  limit of search interval
#define RIGHT_LIMIT         10.0    // right limit of search interval

#define MEAN                 0.0    // mean of the sites distribution
#define SIGMA                1.0    // sandard deviation of the sites
                                    // distribution

int main()
{
    DFTaskPtr task;                 // Data Fitting task descriptor
    MKL_INT nx;                     // number of break points
    MKL_INT xhint;                  // additional info about break points
    MKL_INT nsite_bl;               // number of search sites in one block
    MKL_INT nsite;                  // total number of sites
    MKL_INT nblocks;                // number of blocks of sites
    MKL_INT sitehint;               // search sites storage format
    MKL_INT cell[NSITE];            // indices of cells containing
    MKL_INT *cell_ptr;              // search sites
    double x[N];                    // array of break points
    double site[NSITE];             // array of search sites
    double *site_ptr;
    double datahint[LDATA];         // additional info about the structure
                                    // of search sites
    double a, sigma;                // parameters of the search sites
                                    // distribution
    double left = LEFT_LIMIT;       // left  limit of the search interval
    double right = RIGHT_LIMIT;     // right limit of the search interval

    MKL_INT test_cell[NSITE];
    int i, j, errcode = 0;
    int errnums = 0;
    double r;

    /***** Initializing parameters for Data Fitting task *****/

    /***** Parameters describing search interval *****/
    nx        = N;
    xhint     = DF_NON_UNIFORM_PARTITION;

    /***** Parameter describing number of search sites *****/
    nsite_bl  = NSITE_BLOCK;
    nsite     = NSITE;
    /* No additional info is provided in case of cell search */
    sitehint  = DF_NO_HINT;

    /***** Generate uniformly distributed search sites *****/
    errcode = dUniformRandSortedData( x, left, right, nx );
    CheckDfError(errcode);

    /***** Parameter describing additional info about the search sites *****/
    /* Presumably most likely cell is cell number (N/2 + 1) */
    datahint[0] = 1.0;
    datahint[1] = (double)DF_APRIORI_MOST_LIKELY_CELL;
    datahint[2] = 0.0;
    datahint[3] = 1.0;
    datahint[4] = (double)((nx/2) + 1);

    /***** Generate normally distributed break points *****/
    a         = MEAN;
    sigma     = SIGMA;
    errcode = dPeakData( site, a, sigma, nsite );
    CheckDfError(errcode);

    /***** Create Data Fitting task *****/
    errcode = dfdNewTask1D( &task, nx, x, xhint, 0, 0, 0 );
    CheckDfError(errcode);

    /***** Perform cells search *****/

    nblocks = NBLOCKS;

    site_ptr = site;
    cell_ptr = cell;
    for ( i = 0; i < nblocks; i++ )
    {
        errcode = dfdSearchCells1D( task, DF_METHOD_STD, nsite_bl, site_ptr,
                                    sitehint, datahint, cell_ptr );
        CheckDfError(errcode);

        site_ptr += nsite_bl;
        cell_ptr += nsite_bl;
    }

    /***** Delete Data Fitting task *****/
    errcode = dfDeleteTask( &task );
    CheckDfError(errcode);

    /***** Check search results *****/
    errcode = dFindCells( nx, x, nsite, site, test_cell );

    for ( i = 0; i < nsite; i++ )
    {
        if ( test_cell[i] != cell[i] ) errnums++;
    }

    /***** Print results *****/
    printf("Number of break points : %d\n", (int)nx);
    printf("Number of search sites : %d\n", (int)nsite);

    /***** Print given function *****/
    printf("\n  X:\n");

    for( j = 0; j < nx; j++ )
        printf(" %+lf\n", x[j]);

    printf("\nResults of cell search:\n");
    printf("    Site          Computed idx      Expected idx\n");
    for ( i = 0; i < nsite; i++ )
    {
        printf(" %+11.6lf   %15d   %15d\n", site[i], (int)(cell[i]),
            (int)(test_cell[i]));
    }

    /***** Print summary of the test *****/
    if (errnums != 0)
    {
        printf("\n\nError: Computed cell indices are incorrect\n");
        return 1;
    }
    else
    {
        printf("\n\nComputed cell indices are correct\n");
    }

    return 0;
}
