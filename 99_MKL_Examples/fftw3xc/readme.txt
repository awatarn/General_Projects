!===============================================================================
! Copyright 2011-2017 Intel Corporation All Rights Reserved.
!
! The source code,  information  and material  ("Material") contained  herein is
! owned by Intel Corporation or its  suppliers or licensors,  and  title to such
! Material remains with Intel  Corporation or its  suppliers or  licensors.  The
! Material  contains  proprietary  information  of  Intel or  its suppliers  and
! licensors.  The Material is protected by  worldwide copyright  laws and treaty
! provisions.  No part  of  the  Material   may  be  used,  copied,  reproduced,
! modified, published,  uploaded, posted, transmitted,  distributed or disclosed
! in any way without Intel's prior express written permission.  No license under
! any patent,  copyright or other  intellectual property rights  in the Material
! is granted to  or  conferred  upon  you,  either   expressly,  by implication,
! inducement,  estoppel  or  otherwise.  Any  license   under such  intellectual
! property rights must be express and approved by Intel in writing.
!
! Unless otherwise agreed by Intel in writing,  you may not remove or alter this
! notice or  any  other  notice   embedded  in  Materials  by  Intel  or Intel's
! suppliers or licensors in any way.
!===============================================================================

mkl/examples/fftw3xc directory contains examples of using FFTW3 C interface to
compute various FFT problems.

Each example is a self-contained C program.

The examples are named by the name of the plan function they use.  Prefixes dp_
and sp_ indicate the floating point precision used in the example, double or
single precision, respectively.  For the FFT problems that Intel(R) MKL does not support
examples are not provided.

For every FFT computed in the example an initialization and verification
function is provided. Initialization function shows how the input data is
indexed and what input will produce a unit peak in the result. Verification
function checks if the unit peak is produced by the computation.

Every example uses dynamically allocated arrays for the data.  Sizes of the
transforms and parameters for verification are selected randomly.

Refer to FFTW3 documentation for detailed description of the functions used in
these examples.  Refer to Intel(R) MKL Reference Manual for limitations of the FFTW3
interface provided by Intel MKL.

Your feedback on the examples is welcome at Intel(R) MKL Forum site:
http://software.intel.com/en-us/forums/intel-math-kernel-library


(set-fill-column 79)
