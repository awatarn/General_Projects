/*
SOLVE AX = B, where matrix A is a tridiagonal matrix.
All the provided implementations of the tridiagonal matrix algorithm assume that
the three diagonals, a (below), b (main), and c (above), are passed as arguments.

Validation:
A =

     1     4     0     0     0
     3     4     1     0     0
     0     2     3     4     0
     0     0     1     3     1
     0     0     0     2     3

B = [1 1 1 1 1]^T

X =

   0.209302325581395
   0.197674418604651
  -0.418604651162791
   0.465116279069767
   0.023255813953488

REF:
[1] https://en.wikibooks.org/wiki/Algorithm_Implementation/Linear_Algebra/Tridiagonal_matrix_algorithm
Last update Sep 16, 2017.
Author: A. Wisitsorasak

*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "mkl_vsl.h"
#include "mkl.h"

void solve_tridiagonal_in_place_destructive(double * x, int X, double * a, double *  b, double *  c);

int main(void){
  int i,Nx = 5;
  double a[Nx], b[Nx], c[Nx], x[Nx];
  a[1] = 3;
  a[2] = 2;
  a[3] = 1;
  a[4] = 2;

  b[0] = 1;
  b[1] = 4;
  b[2] = 3;
  b[3] = 3;
  b[4] = 3;

  c[0] = 4;
  c[1] = 1;
  c[2] = 4;
  c[3] = 1;

  for(i=0;i<Nx;i++) x[i] = 1.0;
  printf("Checkpoint 1\n");
  solve_tridiagonal_in_place_destructive(&x[0], Nx, &a[0], &b[0], &c[0]);
  printf("Checkpoint 2\n");
  for(i=0;i<Nx;i++)
    printf("x[%d] = %15.8f\n",i,x[i]);
  printf("Done.\n");
	return 0;
}

void solve_tridiagonal_in_place_destructive(double * x, int X, double * a, double *  b, double *  c) {
    /*
     solves Ax = v where A is a tridiagonal matrix consisting of vectors a, b, c
     x - initially contains the input vector v, and returns the solution x. indexed from 0 to X - 1 inclusive
     X - number of equations (length of vector x)
     a - subdiagonal (means it is the diagonal below the main diagonal), indexed from 1 to X - 1 inclusive
     b - the main diagonal, indexed from 0 to X - 1 inclusive
     c - superdiagonal (means it is the diagonal above the main diagonal), indexed from 0 to X - 2 inclusive

     Note: contents of input vector c will be modified, making this a one-time-use function (scratch space can be allocated instead for this purpose to make it reusable)
     Note 2: We don't check for diagonal dominance, etc.; this is not guaranteed stable
     */
    int ix;
    c[0] = c[0] / b[0];
    x[0] = x[0] / b[0];
    /* loop from 1 to X - 1 inclusive, performing the forward sweep */
    for (ix = 1; ix < X; ix++) {
        double m = 1.0/(b[ix] - a[ix] * c[ix - 1]);
        c[ix] = c[ix] * m;
        x[ix] = (x[ix] - a[ix] * x[ix - 1]) * m;
    }
    /* loop from X - 2 to 0 inclusive (safely testing loop condition for an  integer), to perform the back substitution */
    for (ix = (X - 2); ix>=0 ; ix--){
        x[ix] -= c[ix] * x[ix + 1];
    }

}
