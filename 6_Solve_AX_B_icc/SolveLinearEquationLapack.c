/*
SOLVE AX = B
REF:
[1] http://software.intel.com/sites/products/documentation/hpc/mkl/mklman/GUID-A02DB70F-9704-42A4-9071-D409D783D911.htm#GUID-A02DB70F-9704-42A4-9071-D409D783D911
[2] http://software.intel.com/sites/products/documentation/hpc/mkl/mklman/GUID-73425BCA-BFB6-4E53-82DA-4EEF729853B0.htm#GUID-73425BCA-BFB6-4E53-82DA-4EEF729853B0
[3] http://software.intel.com/en-us/forums/topic/280915
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "mkl_vsl.h"
#include "mkl.h"
#include <mkl_lapacke.h>

int main(){
	printf("Start\n");
	double A[5][5], B[5], X[5];
  int i, j;
	// SETUP MATRIX A
	for(j=0;j<5;j++){
		for(i=0;i<5;i++){
			A[j][i] = 0;
		}
	}
	A[0][0] = 1;
	A[0][1] = 2;
	A[1][0] = 4;
	A[1][1] = 5;
	A[1][2] = 6;
	A[2][1] = 7;
	A[2][2] = 8;
	A[2][3] = 9;
	A[3][2] = 3;
	A[3][3] = 2;
	A[3][4] = 1;
	A[4][3] = 5;
	A[4][4] = 4;
	// SETUP VECTOR B
	for(j=0;j<5;j++){
		B[j] = j+1;
	}
	printf("A = \n");
	for(j=0;j<5;j++){
		for(i=0;i<5;i++){
			printf("%.2f ",A[j][i]);
		}
    printf("\n");
	}
  printf("\n");
	printf("A = \n");
	for(j=0;j<5;j++){
		for(i=0;i<5;i++){
			printf("%.2f ",A[j][i]);
		}
		printf("\t| %.2f\n",B[j]);
	}

	int info1, info2;
	long long ipiv[5];
	printf("LAPACK_ROW_MAJOR = %d\n",LAPACK_ROW_MAJOR);
	// LU FACTORIZATION
	info1=LAPACKE_dgetrf(LAPACK_ROW_MAJOR,5,5,&A[0][0],5,&(ipiv[0]));
	printf("A = \n");
	for(j=0;j<5;j++){
		for(i=0;i<5;i++){
			printf("%.2f ",A[j][i]);
		}
		printf("\t| %.2f\n",B[j]);
	}
	// SOLVE THE LINEAR EQUATION
	info2 = LAPACKE_dgetrs(LAPACK_ROW_MAJOR,'N',5,1,&A[0][0],5,&(ipiv[0]),&B[0],1);
	// THE SOLUTION IS OVERWRITTEN ON MATRIX B
	printf("A = \n");
	for(j=0;j<5;j++){
		for(i=0;i<5;i++){
			printf("%.2f ",A[j][i]);
		}
		printf("\t| %.4f\n",B[j]);
	}

	printf("Done\n");
	return 0;
}
