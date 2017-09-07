/*******************************************************************************
* Copyright 2014-2017 Intel Corporation All Rights Reserved.
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
// Calculate double precision GaussianMV sample characteristics
*/
void dCalculateGaussianMVSampleCharacteristics(int n, int dim, double * r,
        double * dbS, double * dbS2, double * dbMean, double * dbVar,
        double * dbCovXYPtr, double * dbCovXZPtr, double * dbCovYZPtr)
{
    int i, j;
    /* Sample characteristics */
    double dbCovXY, dbCovXZ, dbCovYZ;

    double dbSXY, dbSXZ, dbSYZ;
    double dn = (double)n;

    for(j=0;j<dim;j++) {
        dbS[j]=0.0;
        dbS2[j]=0.0;
    }
    dbSXY=dbSXZ=dbSYZ=0.0;
    for(i=0;i<n;i++) {
        for(j=0;j<dim;j++) {
            dbS[j] += r[i*dim + j];
            dbS2[j] += r[i*dim + j]*r[i*dim + j];
        }
        dbSXY += r[i*dim + 0]*r[i*dim + 1];
        dbSXZ += r[i*dim + 0]*r[i*dim + 2];
        dbSYZ += r[i*dim + 1]*r[i*dim + 2];
    }
    for(j=0;j<dim;j++) {
        dbMean[j] = dbS[j]/dn;
        dbVar[j] = dbS2[j]/dn-dbMean[j]*dbMean[j];
    }
    dbCovXY = dbSXY/dn-dbMean[0]*dbMean[1];
    dbCovXZ = dbSXZ/dn-dbMean[0]*dbMean[2];
    dbCovYZ = dbSYZ/dn-dbMean[1]*dbMean[2];
    *dbCovXYPtr = dbCovXY;
    *dbCovXZPtr = dbCovXZ;
    *dbCovYZPtr = dbCovYZ;
}

/*
// Calculate single precision GaussianMV sample characteristics
*/
void sCalculateGaussianMVSampleCharacteristics(int n, int dim, float * r,
        float * dbS, float * dbS2, float * dbMean, float * dbVar,
        float * dbCovXYPtr, float * dbCovXZPtr, float * dbCovYZPtr)
{
    int i, j;
    /* Sample characteristics */
    float dbCovXY, dbCovXZ, dbCovYZ;

    float dbSXY, dbSXZ, dbSYZ;
    float dn = (float)n;

    for(j=0;j<dim;j++) {
        dbS[j]=0.0f;
        dbS2[j]=0.0f;
    }
    dbSXY=dbSXZ=dbSYZ=0.0f;
    for(i=0;i<n;i++) {
        for(j=0;j<dim;j++) {
            dbS[j] += r[i*dim + j];
            dbS2[j] += r[i*dim + j]*r[i*dim + j];
        }
        dbSXY += r[i*dim + 0]*r[i*dim + 1];
        dbSXZ += r[i*dim + 0]*r[i*dim + 2];
        dbSYZ += r[i*dim + 1]*r[i*dim + 2];
    }
    for(j=0;j<dim;j++) {
        dbMean[j] = dbS[j]/dn;
        dbVar[j] = dbS2[j]/dn-dbMean[j]*dbMean[j];
    }
    dbCovXY = dbSXY/dn-dbMean[0]*dbMean[1];
    dbCovXZ = dbSXZ/dn-dbMean[0]*dbMean[2];
    dbCovYZ = dbSYZ/dn-dbMean[1]*dbMean[2];
    *dbCovXYPtr = dbCovXY;
    *dbCovXZPtr = dbCovXZ;
    *dbCovYZPtr = dbCovYZ;
}

/*
// Check that double precision GaussianMV sample characteristics agree with theory
*/
int dGaussianMVCheckResults(int dim, int n, double *a, double *CC, double *dbMean, double *dbVar,
        double *S, double *D2, double *Q, double *DeltaM, double *DeltaD)
{
    int j, status = 0;
    double dn = (double)n;
    for(j=0;j<dim;j++) {
        D2[j] = CC[j*dim + j]*CC[j*dim + j];
        Q[j]  = 3.0*D2[j];
        S[j]  = ((Q[j]-D2[j])/dn)-(2*(Q[j]-2*D2[j])/(dn*dn))+((Q[j]-3*D2[j])/(dn*dn*dn));
        DeltaM[j] = (a[j]-dbMean[j])/sqrt(CC[j*dim + j]/dn);
        DeltaD[j] = (CC[j*dim + j]-dbVar[j])/sqrt(S[j]);
        /* All deltas must be less than 3.0 */
        if(fabs(DeltaM[j])>3.0 || fabs(DeltaD[j])>3.0) {
            status = 1;
        }
    }
    return status;
}

/*
// Check that single precision GaussianMV sample characteristics agree with theory
*/
int sGaussianMVCheckResults(int dim, int n, float *a, float *CC, float *dbMean, float *dbVar,
        float *S, float *D2, float *Q, float *DeltaM, float *DeltaD)
{
    int j, status = 0;
    float dn = (float)n;
    for(j=0;j<dim;j++) {
        D2[j] = CC[j*dim + j]*CC[j*dim + j];
        Q[j]  = 3.0*D2[j];
        S[j]  = ((Q[j]-D2[j])/dn)-(2*(Q[j]-2*D2[j])/(dn*dn))+((Q[j]-3*D2[j])/(dn*dn*dn));
        DeltaM[j] = (a[j]-dbMean[j])/sqrt(CC[j*dim + j]/dn);
        DeltaD[j] = (CC[j*dim + j]-dbVar[j])/sqrt(S[j]);
        /* All deltas must be less than 3.0 */
        if(fabs(DeltaM[j])>3.0 || fabs(DeltaD[j])>3.0) {
            status = 1;
        }
    }
    return status;
}
