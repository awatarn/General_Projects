/*******************************************************************************
* Copyright 2015-2017 Intel Corporation All Rights Reserved.
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

#include <stdio.h>
#include <stdlib.h>

#include "mkl_dnn.h"

#define CHECK_ERR(f, err) do { \
    (err) = (f); \
    if ((err) != E_SUCCESS) { \
        printf("[%s:%d] err (%d)\n", __FILE__, __LINE__, err); \
        goto bail_out; \
    } \
} while(0)

#define dimension (4)

static dnnError_t init_conversion(dnnPrimitive_t *cv, double **ptr_out,
                                 dnnLayout_t lt_pr, dnnLayout_t lt_us, double *ptr_us)
{
    dnnError_t err;
    *ptr_out = NULL;
    if (!dnnLayoutCompare_F64(lt_pr, lt_us)) {
        CHECK_ERR( dnnConversionCreate_F64(cv, lt_us, lt_pr), err );
        CHECK_ERR( dnnAllocateBuffer_F64((void**)ptr_out, lt_pr), err );
    } else {
        *ptr_out = ptr_us;
    }
    return E_SUCCESS;

bail_out:
    if (*ptr_out) dnnReleaseBuffer_F64(*ptr_out);
    return err;
}

static dnnError_t simple_net() {
    dnnError_t err;
    size_t batch_size = 16;

    size_t outputSize[dimension] = { 45, 45, 16, batch_size };
    size_t outputStrides[dimension] = { 1, 45, 45 * 45, 45 * 45 * 16 };

    size_t inputSize[dimension] = { 49, 49, 16, batch_size };
    size_t inputStrides[dimension] = { 1, 49, 49 * 49, 49 * 49 * 16 };

    size_t filterSize[dimension] = { 5, 5, 16, 16 };
    size_t filterStrides[dimension] = { 1, 5, 5 * 5, 5 * 5 * 16 };

    size_t convolutionStride[dimension - 2] = { 1, 1 };
    int inputOffset[dimension - 2 ] = { 0, 0 };

    size_t biasSize[1] = { outputSize[2] };
    size_t biasStrides[1] = { outputStrides[2] };

    size_t pool_outputSize[dimension] = { 22, 22, 16, batch_size };
    size_t pool_outputStrides[dimension] = { 1, 22, 22 * 22, 22 * 22 * 16 };

    dnnLayout_t lt_user_input = NULL,
                lt_user_filt = NULL,
                lt_user_bias = NULL,
                lt_user_output = NULL;
    double* resConv1[dnnResourceNumber] = {0};
    double* resPool1[dnnResourceNumber] = {0};
    dnnPrimitive_t conv1 = NULL;
    dnnLayout_t lt_conv1_input = NULL,
                lt_conv1_filt = NULL,
                lt_conv1_bias = NULL,
                lt_conv1_output = NULL;
    dnnPrimitive_t cv_user_to_conv1_input = NULL,
                   cv_user_to_conv1_filt = NULL,
                   cv_user_to_conv1_bias = NULL;
    dnnPrimitive_t conv_bwd_data = NULL;
    dnnLayout_t lt_conv_diff_src = NULL;
    dnnPrimitive_t conv_bwd_filter = NULL;
    dnnLayout_t lt_conv_diff_filter = NULL;
    dnnPrimitive_t conv_bwd_bias = NULL;
    dnnLayout_t lt_conv_diff_bias = NULL;
    size_t kernelSize[2] = { 3, 3 };
    size_t kernelStride[2] = { 2, 2 };
    dnnLayout_t lt_pool1_input = NULL;
    dnnPrimitive_t pool1 = NULL;
    dnnLayout_t lt_pool1_output = NULL,
                lt_pool1_workspace = NULL;
    dnnPrimitive_t cv_pool_to_user_output = NULL,
                   cv_user_to_pool_output = NULL;
    dnnPrimitive_t pool_bwd = NULL;
    dnnLayout_t lt_pool_diff = NULL;
    dnnPrimitiveAttributes_t attributes = NULL;

    double *user_i = NULL,
           *user_c1_f = NULL,
           *user_c1_b = NULL,
           *user_o = NULL;

    int k;


    /*** data allocation ***/
    user_i = (double*)malloc(sizeof(double)*(8 * 1024 * 1024));
    user_c1_f = (double*)malloc(sizeof(double)*(8 * 1024 * 1024));
    user_c1_b = (double*)malloc(sizeof(double)*(8 * 1024 * 1024));
    if (user_i == NULL || user_c1_f == NULL || user_c1_b == NULL) {
        err = E_MEMORY_ERROR;
        goto bail_out;
    }

    /*** User's data description ***/
    CHECK_ERR( dnnLayoutCreate_F64(&lt_user_input, dimension, inputSize, inputStrides) , err );
    CHECK_ERR( dnnLayoutCreate_F64(&lt_user_filt, dimension, filterSize, filterStrides), err );
    CHECK_ERR( dnnLayoutCreate_F64(&lt_user_bias, 1, biasSize, biasStrides) , err );
    CHECK_ERR( dnnLayoutCreate_F64(&lt_user_output, dimension,
                               pool_outputSize, pool_outputStrides), err );

    /* Initialize attributes */
    CHECK_ERR( dnnPrimitiveAttributesCreate_F64(&attributes), err );
 
    /*** convolution section ***/
    CHECK_ERR( dnnConvolutionCreateForwardBias_F64(&conv1, attributes,
                dnnAlgorithmConvolutionDirect, dimension, inputSize,
                outputSize, filterSize, convolutionStride, inputOffset,
                dnnBorderZeros), err);

    // Convolution describes what layout it expects
    CHECK_ERR( dnnLayoutCreateFromPrimitive_F64(&lt_conv1_input, conv1, dnnResourceSrc) , err );
    CHECK_ERR( dnnLayoutCreateFromPrimitive_F64(&lt_conv1_filt, conv1, dnnResourceFilter), err );
    CHECK_ERR( dnnLayoutCreateFromPrimitive_F64(&lt_conv1_bias, conv1, dnnResourceBias) , err );
    CHECK_ERR( dnnLayoutCreateFromPrimitive_F64(&lt_conv1_output,conv1, dnnResourceDst) , err );

    CHECK_ERR( init_conversion(&cv_user_to_conv1_input, &resConv1[dnnResourceSrc], lt_conv1_input, lt_user_input, user_i) , err );
    CHECK_ERR( init_conversion(&cv_user_to_conv1_filt, &resConv1[dnnResourceFilter], lt_conv1_filt, lt_user_filt, user_c1_f), err );
    CHECK_ERR( init_conversion(&cv_user_to_conv1_bias, &resConv1[dnnResourceBias], lt_conv1_bias, lt_user_bias, user_c1_b), err );
    CHECK_ERR( dnnAllocateBuffer_F64((void**)&resConv1[dnnResourceDst], lt_conv1_output), err );

    /*  Convolution backward data */
    CHECK_ERR( dnnConvolutionCreateBackwardData_F64 (&conv_bwd_data, attributes,
                dnnAlgorithmConvolutionDirect, dimension, inputSize,
                outputSize, filterSize, convolutionStride, inputOffset,
                dnnBorderZeros), err);


    CHECK_ERR( dnnLayoutCreateFromPrimitive_F64(&lt_conv_diff_src, conv_bwd_data,
               dnnResourceDiffSrc), err );
    CHECK_ERR( dnnAllocateBuffer_F64((void**)&resConv1[dnnResourceDiffSrc],
                lt_conv_diff_src), err );

    /*  Convolution backward filter  */
    CHECK_ERR( dnnConvolutionCreateBackwardFilter_F64 (&conv_bwd_filter, attributes,
                dnnAlgorithmConvolutionDirect, dimension, inputSize,
                outputSize, filterSize, convolutionStride, inputOffset,
                dnnBorderZeros), err);

    CHECK_ERR( dnnLayoutCreateFromPrimitive_F64(&lt_conv_diff_filter, conv_bwd_filter,
                dnnResourceDiffFilter), err );
    CHECK_ERR( dnnAllocateBuffer_F64((void**)&resConv1[dnnResourceDiffFilter],
                lt_conv_diff_filter), err );

    /*  Convolution backward bias  */
    CHECK_ERR( dnnConvolutionCreateBackwardBias_F64 (&conv_bwd_bias, attributes,
               dnnAlgorithmConvolutionDirect, dimension, outputSize), err);

    CHECK_ERR( dnnLayoutCreateFromPrimitive_F64(&lt_conv_diff_bias, conv_bwd_bias,
                dnnResourceDiffBias), err );
    CHECK_ERR( dnnAllocateBuffer_F64((void**)&resConv1[dnnResourceDiffBias],
                lt_conv_diff_bias), err );

    /*** Pooling section ***/
    lt_pool1_input = lt_conv1_output;

    CHECK_ERR( dnnPoolingCreateForward_F64(&pool1, attributes, dnnAlgorithmPoolingMax,
        lt_pool1_input, kernelSize, kernelStride, inputOffset, dnnBorderZeros), err );

    resPool1[dnnResourceSrc] = resConv1[dnnResourceDst];

    CHECK_ERR( dnnLayoutCreateFromPrimitive_F64(&lt_pool1_output, pool1, dnnResourceDst), err );
    CHECK_ERR( dnnLayoutCreateFromPrimitive_F64(&lt_pool1_workspace, pool1, dnnResourceWorkspace), err );
    CHECK_ERR( dnnAllocateBuffer_F64((void**)&resPool1[dnnResourceDst], lt_pool1_output) , err );
    CHECK_ERR( dnnAllocateBuffer_F64((void**)&resPool1[dnnResourceWorkspace], lt_pool1_workspace) , err );

    CHECK_ERR( init_conversion(&cv_pool_to_user_output, &user_o, lt_user_output, lt_pool1_output, resPool1[dnnResourceDst]) , err );
    CHECK_ERR( init_conversion(&cv_user_to_pool_output, &resPool1[dnnResourceDiffDst], lt_pool1_output, lt_user_output, user_o) , err );

    /* Pooling backward */
    CHECK_ERR( dnnPoolingCreateBackward_F64(&pool_bwd, attributes, dnnAlgorithmPoolingMax,
                lt_pool1_input, kernelSize, kernelStride, inputOffset,
                dnnBorderZeros), err );

    CHECK_ERR( dnnLayoutCreateFromPrimitive_F64(&lt_pool_diff, pool_bwd,
                dnnResourceDiffSrc), err );
    CHECK_ERR( dnnAllocateBuffer_F64((void**)&resPool1[dnnResourceDiffSrc],
                lt_pool_diff), err );

    // This connect Pool Diff Src with Conv Diff Dst
    resConv1[dnnResourceDiffDst] = resPool1[dnnResourceDiffSrc];

    /*** Execution ***/
    if (cv_user_to_conv1_filt) CHECK_ERR( dnnConversionExecute_F64(cv_user_to_conv1_filt, user_c1_f, resConv1[dnnResourceFilter]), err );
    if (cv_user_to_conv1_bias) CHECK_ERR( dnnConversionExecute_F64(cv_user_to_conv1_bias, user_c1_b, resConv1[dnnResourceBias]) , err );

    for (k = 0; k < 1; ++k) {
        if (cv_user_to_conv1_input) CHECK_ERR( dnnConversionExecute_F64(cv_user_to_conv1_input, user_i, resConv1[dnnResourceSrc]), err );

        CHECK_ERR( dnnExecute_F64(conv1, (void*)resConv1), err );
        CHECK_ERR( dnnExecute_F64(pool1, (void*)resPool1), err );

        if (cv_pool_to_user_output) CHECK_ERR( dnnConversionExecute_F64(cv_pool_to_user_output, resPool1[dnnResourceDst], user_o), err );
        // Calculation of output diff and storing results into user_o
        if (cv_user_to_pool_output) CHECK_ERR( dnnConversionExecute_F64(cv_user_to_pool_output, user_o, resPool1[dnnResourceDiffDst]), err );

        CHECK_ERR( dnnExecute_F64(pool_bwd, (void*)resPool1), err );
        CHECK_ERR( dnnExecute_F64(conv_bwd_data, (void*)resConv1), err );
        CHECK_ERR( dnnExecute_F64(conv_bwd_filter, (void*)resConv1), err );
        CHECK_ERR( dnnExecute_F64(conv_bwd_bias, (void*)resConv1), err );

        // Applying updates and shifting pointers
    }

bail_out:

    dnnDelete_F64(conv1);
    dnnDelete_F64(conv_bwd_data);
    dnnDelete_F64(conv_bwd_filter);
    dnnDelete_F64(conv_bwd_bias);
    dnnDelete_F64(pool1);
    dnnDelete_F64(pool_bwd);
    dnnDelete_F64(cv_user_to_conv1_input);
    dnnDelete_F64(cv_user_to_conv1_filt);
    dnnDelete_F64(cv_user_to_conv1_bias);
    dnnDelete_F64(cv_pool_to_user_output);
    dnnDelete_F64(cv_user_to_pool_output);

    dnnLayoutDelete_F64(lt_user_input);
    dnnLayoutDelete_F64(lt_user_filt);
    dnnLayoutDelete_F64(lt_user_bias);
    dnnLayoutDelete_F64(lt_user_output);
    dnnLayoutDelete_F64(lt_conv1_input);
    dnnLayoutDelete_F64(lt_conv1_filt);
    dnnLayoutDelete_F64(lt_conv1_bias);
    dnnLayoutDelete_F64(lt_conv1_output);
    dnnLayoutDelete_F64(lt_conv_diff_src);
    dnnLayoutDelete_F64(lt_conv_diff_filter);
    dnnLayoutDelete_F64(lt_conv_diff_bias);
    dnnLayoutDelete_F64(lt_pool1_output);
    dnnLayoutDelete_F64(lt_pool1_workspace);
    dnnLayoutDelete_F64(lt_pool_diff);

    dnnPrimitiveAttributesDestroy_F64(attributes);
    if (resConv1[dnnResourceSrc] != (void*)user_i) dnnReleaseBuffer_F64(resConv1[dnnResourceSrc]);
    if (resConv1[dnnResourceFilter] != (void*)user_c1_f) dnnReleaseBuffer_F64(resConv1[dnnResourceFilter]);
    if (resConv1[dnnResourceBias] != (void*)user_c1_b) dnnReleaseBuffer_F64(resConv1[dnnResourceBias]);
    dnnReleaseBuffer_F64(resConv1[dnnResourceDst]);
    dnnReleaseBuffer_F64(resConv1[dnnResourceDiffSrc]);
    dnnReleaseBuffer_F64(resConv1[dnnResourceDiffFilter]);
    dnnReleaseBuffer_F64(resConv1[dnnResourceDiffBias]);
    dnnReleaseBuffer_F64(resPool1[dnnResourceDst]);
    dnnReleaseBuffer_F64(resPool1[dnnResourceWorkspace]);
    dnnReleaseBuffer_F64(resPool1[dnnResourceDiffSrc]);
    if ((void*)user_o != resPool1[dnnResourceDst]) dnnReleaseBuffer_F64(user_o);
    if (resPool1[dnnResourceDiffDst] != (void*)user_o) dnnReleaseBuffer_F64(resPool1[dnnResourceDiffDst]);

    free(user_i);
    free(user_c1_f);
    free(user_c1_b);

    return err;
}

int main(int argc, char **argv) {
    dnnError_t err;
    err = simple_net();
    if (err != E_SUCCESS) { printf("FAILED\n"); return err; }

    printf("PASSED\n");
    return 0;
}
