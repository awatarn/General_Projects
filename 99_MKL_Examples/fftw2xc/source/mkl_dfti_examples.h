/*******************************************************************************
* Copyright 2006-2017 Intel Corporation All Rights Reserved.
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
! Content:
!      Intel(R) MKL DFTI examples definitions file (C-interface)
!
!****************************************************************************/

#ifndef MKL_DFTI_EXAMPLES_H_INCLUDED
#define MKL_DFTI_EXAMPLES_H_INCLUDED

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/*
**  Print level definition
*/
#define  ACCURACY_PRINT 1
#define  LEGEND_PRINT 1

/*
**  Static array's size definition
*/
#define M_MAX 15
#define N_MAX 10
#define K_MAX 12
#define L_MAX 10
#define M_MAX4 5
#define N_MAX4 3
#define K_MAX4 2
#define L_MAX4 4

/*
**  Accuracy definitions
*/

#define SINGLE_EPS 1.0E-6
#define DOUBLE_EPS 1.0E-12

/*
**  Constant's definition
*/
#define  MATH_PI     3.14159265358979323846  /* pi */

/* Intel(R) MKL test _Complex type definition */
typedef struct {
    float re;
    float im;
} mkl_float_complex;

typedef struct {
    double re;
    double im;
} mkl_double_complex;

#ifdef MKL_DOUBLE
    #define MKL_PRECISION mkl_double_complex
    #define TYPE_PRECISION double
    #define EPS DOUBLE_EPS
#else
    #define MKL_PRECISION mkl_float_complex
    #define TYPE_PRECISION float
    #define EPS SINGLE_EPS
#endif

/*
**  Example support function's interfaces
*/

int init_input_and_expected_vectors_c(void* x_in, void* x_exp, long n);
int init_input_and_expected_vectors_z(void* x_in, void* x_exp, long n);

int zero_init_c(void* x_in, long n);
int zero_init_z(void* x_in, long n);

int zero_init_s(void* x_in, long n);
int zero_init_d(void* x_in, long n);

int init_real_vectors_s(void*  x_in, long n);
int init_real_vectors_d(void* x_in, long n);

int init_multiple_columns_c(void* x, long n, long multiple,
                            long first_index, long step_x);
int init_multiple_columns_z(void* x, long n, long multiple,
                            long first_index, long step_x);
void init_multiple_columns_step_c(void* x, long n, long multiple,
                                  long step_y, long step_x);

void init_multiple_columns_step_z(void* x, long n, long multiple,
                             long step_y, long step_x);

int init_3d_columns_c(void* x, long m, long n, long k,
                      long* strides);
int init_3d_columns_z(void* x, long m, long n, long k,
                      long* strides);

float  check_result_c(void* x_in, void* x_exp, long n);
double check_result_z(void* x_in, void* x_exp, long n);

float  check_result_s(void* x_in, void* x_exp, long n);
double check_result_d(void* x_in, void* x_exp, long n);
TYPE_PRECISION check_result_multiple(void* x_in, void* x_exp, int n, int multiple,
                                     int step_y, int step_x);

TYPE_PRECISION check_result_2d_r(TYPE_PRECISION*, TYPE_PRECISION*, int, int);
TYPE_PRECISION check_result_3d_r(TYPE_PRECISION*, TYPE_PRECISION*, int, int, int);
TYPE_PRECISION check_result_2d_r_multiple(TYPE_PRECISION*, TYPE_PRECISION*, int, int, int, int, int);
TYPE_PRECISION check_result_3d_r_multiple(TYPE_PRECISION*, TYPE_PRECISION*, int, int, int, int, int, int);

void cblas_ccopy(int, void*, int, void*, int);
void cblas_zcopy(int, void*, int, void*, int);
void cblas_scopy(int, const float*, int, float*, int);
void cblas_dcopy(int, const double*, int, double*, int);
void scaling(void*, TYPE_PRECISION, int);
void scaling_multiple(void*, TYPE_PRECISION, int, int, int, int);
void scaling_multiple_2d(void*, TYPE_PRECISION, int, int, int, int, int);

void scaling_r(void*, TYPE_PRECISION, int);
void scaling_r_multiple(void* xx, TYPE_PRECISION Scale,
                        int n, int multiple, int istride, int idist);
void scaling_2d_r(TYPE_PRECISION*, TYPE_PRECISION, int, int);
void scaling_3d_r(TYPE_PRECISION*, TYPE_PRECISION, int, int, int);
void scaling_2d_r_multiple(TYPE_PRECISION*, TYPE_PRECISION, int, int, int, int, int);
void scaling_3d_r_multiple(TYPE_PRECISION*, TYPE_PRECISION, int, int, int, int, int, int);

void die_unless(int expr, const char *file, int line, const char *expr_str);
#define DIE_UNLESS(expr) die_unless( !!(expr), __FILE__, __LINE__, #expr )

#endif /* MKL_DFTI_EXAMPLES_H_INCLUDED */
