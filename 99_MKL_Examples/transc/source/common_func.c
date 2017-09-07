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
!
!******************************************************************************/
#include <mkl_types.h>
#include "common_func.h"
/* 
**  Print matrix of any type
*/                          

size_t print_number(void* data, char data_type)
{ 
  switch(data_type)
  {
    case 's':
      printf("%4f\t", *((float*)data));  
      return sizeof(float);
    case 'd':
      printf("%4lf\t", *((double*)data));  
      return sizeof(double);
    case 'c':
      printf("(%4f,%4fi)\t", ((MKL_Complex8*)data)->real, ((MKL_Complex8*)data)->imag);  
      return sizeof(MKL_Complex8);
    case 'z':
      printf("(%4lf,%4lfi)\t", ((MKL_Complex16*)data)->real, ((MKL_Complex16*)data)->imag);  
      return sizeof(MKL_Complex16);
  }
  return 0;
}

void print_matrix(size_t rows, size_t cols, char data_type, void *src)
{
  size_t i=0;
  size_t j=0;
  char* p_char=((char*) src);

  for(i=0; i<rows; i++)
  {
    for(j=0; j<cols; j++)
      p_char += print_number( p_char , data_type);
    printf("\n");
  }
}
