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
!    vslRegisterBrng  Example Program Text
!******************************************************************************/

#include <stdio.h>
#include <math.h>

#include "mkl_vsl.h"
#include "errcheck.inc"

#define N   4

typedef struct _MinStdStreamState {
    unsigned int Reserved1[2];
    unsigned int Reserved2[2];
    unsigned int a;
    unsigned int x;
} MinStdStreamState;

static unsigned int Mod( unsigned int , unsigned int  );
static unsigned int PowMod( unsigned int , int  );
int MinStdInitStream( int , VSLStreamStatePtr , int , const unsigned int [] );
int iMinStd( VSLStreamStatePtr , int , unsigned int [] );
int sMinStd( VSLStreamStatePtr , int , float [], float , float  );
int dMinStd( VSLStreamStatePtr , int , double [], double , double  );

int main()
{
    MKL_INT             brng;
    VSLBRngProperties   properties;
    VSLStreamStatePtr   stream;
    VSLStreamStatePtr   stream1;
    VSLStreamStatePtr   stream2;
    VSLStreamStatePtr   stream3;
    unsigned int        r[3*N];
    unsigned int        r1[N];
    unsigned int        r2[N];
    unsigned int        r3[N];
    int                 i, errcode;

    printf("vslRegisterBrng\nRegistering user-designed BRNG...\n");

    /* Fill the fields of BRNG properties structure */
    properties.StreamStateSize = sizeof( MinStdStreamState );
    properties.NSeeds = 1;
    properties.IncludesZero = 0;
    properties.WordSize = 4;
    properties.NBits = 31;
    properties.InitStream = MinStdInitStream;
    properties.sBRng = sMinStd;
    properties.dBRng = dMinStd;
    properties.iBRng = iMinStd;

    brng = (MKL_INT)vslRegisterBrng( &properties );

    if ( brng < 0 )
    {
        printf( "REGISTRATION ERROR: %i\n", brng );
        return 1;
    }
    else
    {
        printf( "User-designed BRNG was successfully registered.\n"\
               "BRNG Index=%i\n", brng );
    }

    errcode = vslNewStream( &stream, brng, 1 );
    CheckVslError( errcode );
    errcode = vslCopyStream( &stream1, stream );
    CheckVslError( errcode );
    errcode = vslCopyStream( &stream2, stream );
    CheckVslError( errcode );
    errcode = vslCopyStream( &stream3, stream );
    CheckVslError( errcode );
    errcode = vslLeapfrogStream( stream1, 0, 3 );
    CheckVslError( errcode );
    errcode = vslLeapfrogStream( stream2, 1, 3 );
    CheckVslError( errcode );
    errcode = vslLeapfrogStream( stream3, 2, 3 );
    CheckVslError( errcode );

    errcode = viRngUniformBits( 0, stream, 3*N, r );
    CheckVslError( errcode );
    errcode = viRngUniformBits( 0, stream1, N, r1 );
    CheckVslError( errcode );
    errcode = viRngUniformBits( 0, stream2, N, r2 );
    CheckVslError( errcode );
    errcode = viRngUniformBits( 0, stream3, N, r3 );
    CheckVslError( errcode );

    printf( "Single stream\n" );
    for ( i = 0; i < 3*N; i++ )
    {
        printf("%0#10x\n", r[i]);
    }

    printf( "1st node\t2nd node\t3rd node\n" );
    for ( i = 0; i < N; i++ )
    {
        printf( "%0#10x\t%0#10x\t%0#10x\n", r1[i], r2[i], r3[i] );
    }

    errcode = vslDeleteStream( &stream );
    CheckVslError( errcode );
    errcode = vslDeleteStream( &stream1 );
    CheckVslError( errcode );
    errcode = vslDeleteStream( &stream2 );
    CheckVslError( errcode );
    errcode = vslDeleteStream( &stream3 );
    CheckVslError( errcode );

    return 0;
}

static unsigned int Mod( unsigned int a, unsigned int b )
{
    unsigned int a0, a1;
    unsigned int b0, b1;
    double       dbC00, dbC01, dbC10, dbC11;
    double       dbRes;

    a0 = (double)(a & 0xffff);
    a1 = (double)(a >> 16);
    b0 = (double)(b & 0xffff);
    b1 = (double)(b >> 16);

    dbC00 = a0*b0-2147483647.0*floor( a0*b0/2147483647.0 );
    dbC01 = 65536.0*a0*b1-2147483647.0*floor( 65536.0*a0*b1/2147483647.0 );
    dbC10 = 65536.0*a1*b0-2147483647.0*floor( 65536.0*a1*b0/2147483647.0 );
    dbC11 = a1*b1-2147483647.0*floor( a1*b1/2147483647.0 );
    dbC11 += dbC11;
    if ( dbC11 >= 2147483647.0 )
        dbC11 -= 2147483647.0;

    dbRes = dbC00+dbC01;
    if ( dbRes >= 2147483647.0 )
        dbRes -= 2147483647.0;

    dbRes += dbC10;
    if ( dbRes >= 2147483647.0 )
        dbRes -= 2147483647.0;

    dbRes += dbC11;
    if ( dbRes >= 2147483647.0 )
        dbRes -= 2147483647.0;

    return ( (unsigned int)dbRes );
}

static unsigned int PowMod( unsigned int a, int n )
{
    unsigned int a2;
    unsigned int r;  /* result */

    r = 1;

    if (n == 0) return r;

    do {
        if (n & 1)
        {
            r = Mod( r, a );
        }

        n >>= 1;

        a = Mod(a, a);
    } while (n);

    return r;
}


int MinStdInitStream( int method, VSLStreamStatePtr stream, int n, const unsigned int params[] )
{
    MinStdStreamState*  mstream;
    int                 errcode;
    double              a2;
    unsigned int        b1, b2, b;
    unsigned int        seed;

    mstream = (MinStdStreamState*)stream;

    /* Initialize the stream */
    errcode = VSL_ERROR_OK;

    switch ( method )
    {
    case VSL_INIT_METHOD_STANDARD:
        {
            /* Initialize multiplier */
            mstream->a = 16807;

            /* Initialize seed */
            if ( n >= 1 )
            {
                seed = params[0] % 0x7fffffff;
                if ( seed == 0) seed = 1;
                mstream->x = seed;
            }
            else
                mstream->x = 1;

            return errcode;
        }
    case VSL_INIT_METHOD_LEAPFROG:
        {
            /* Compute (x*a^params[0]) mod (2^31-1) */
            mstream->x = Mod( mstream->x, PowMod(mstream->a, params[0]) );

            /* Compute (a^n) mod (2^31-1) */
            mstream->a = PowMod(mstream->a, n);

            return errcode;
        }
    case VSL_INIT_METHOD_SKIPAHEAD:
        {
            /* Compute (x*a^n mod (2^31-1) */
            mstream->x = Mod( mstream->x, PowMod(mstream->a, n) );
            return errcode;
        }
    }

    return errcode;
}

int iMinStd( VSLStreamStatePtr stream, int n, unsigned int r[] )
{
    MinStdStreamState*  mstream;
    int                 i;
    unsigned int        x;

    mstream = (MinStdStreamState*)stream;
    x = mstream->x;

    for ( i = 0; i < n; i++ )
    {
        r[i] = x;
        x = Mod( mstream->a, x );
    }

    mstream->x = x;

    return 0;
}

int sMinStd( VSLStreamStatePtr stream, int n, float r[], float a, float b )
{
    MinStdStreamState*  mstream;
    int                 i;
    unsigned int        x;
    double              c;

    mstream = (MinStdStreamState*)stream;
    x = mstream->x;
    c = (b-a)/2147483647.0;

    for ( i = 0; i < n; i++ )
    {
        r[i] = (float)((double)x*c+a);
        x = Mod( mstream->a, x );
    }

    mstream->x = x;

    return 0;
}

int dMinStd( VSLStreamStatePtr stream, int n, double r[], double a, double b )
{
    MinStdStreamState*  mstream;
    int                 i;
    unsigned int        x;
    double              c;

    mstream = (MinStdStreamState*)stream;
    x = mstream->x;
    c = (b-a)/2147483647.0;

    for ( i = 0; i < n; i++ )
    {
        r[i] = (double)x*c+a;
        x = Mod( mstream->a, x );
    }

    mstream->x = x;

    return 0;
}
