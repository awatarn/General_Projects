/*
SOLVE AX = B, where matrix A is a cyclic-tridiagonal matrix.
All the provided implementations of the tridiagonal matrix algorithm assume that
the three diagonals, a (below), b (main), and c (above), are passed as arguments.

b1*x1 + c1*x2                     + a1*xn = f1
a2*x1 + b2*x2 + c2*x3                     = f2
        a3*x2 + b3*x3 + c3*x4             = f3
                a4*x3 + b4*x4     + c4*x5 = f4

cn*x1                 + an*x(n-1) + bn*xn = fn

Validation:
A =

     1     4     0     0     1
     3     4     1     0     0
     0     2     3     4     0
     0     0     1     3     1
     2     0     0     2     3

B = [1 1 1 1 1]^T

X =

   0.228571428571429
   0.257142857142857
  -0.714285714285714
   0.657142857142857
  -0.257142857142857

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
void Solve_CyclicTridiagonal(double *xx, int Nx, double *aa, double *bb, double *cc);

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

  a[0] = 1;
  c[4] = 2;

  for(i=0;i<Nx;i++) x[i] = 1.0;
  printf("Checkpoint 1\n");
  Solve_CyclicTridiagonal(&x[0], Nx, &a[0], &b[0], &c[0]);
  printf("Checkpoint 2\n");
  for(i=0;i<Nx;i++)
     printf("x[%d] = %15.8f\n",i,x[i]);
  printf("Done.\n");
	return 0;
}

void solve_tridiagonal_in_place_destructive(double * x, int X, double * a, double *  b, double *  c){
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

void Solve_CyclicTridiagonal(double *ff, int Nxx, double *aa, double *bb, double *cc){
  /*
   solves Ax = v where A is a cyclic-tridiagonal matrix consisting of vectors a, b, c
   ff  - initially contains the input vector v, and returns the solution x. indexed from 0 to X - 1 inclusive
   Nxx - number of equations (length of vector x)
   aa  - subdiagonal (means it is the diagonal below the main diagonal), indexed from 1 to X - 1 inclusive
   bb  - the main diagonal, indexed from 0 to X - 1 inclusive
   cc  - superdiagonal (means it is the diagonal above the main diagonal), indexed from 0 to X - 2 inclusive

   b(0)*x(0)   + c(1)*x(1)                             + a(1)*x(N-1)   = f(0)
   a(1)*x(0)   + b(1)*x(1) + c(1)*x(2)                                 = f(1)
                 a(2)*x(1) + b(1)*x(2) + c(1)*x(3)                     = f(2)
   ............................................................................
   c(N-1)*x(0)                         + a(N-1)*x(N-2) + b(N-1)*x(N-1) = f(N-1)

   Ref: A Theoretical Introduction to Numerical Analysis, VS Ryaben'kii & SV Tsynkov, Chapman & Hall/CRC Page 148
   Author: A Wisitsorasak
   Date: 17 Sep 2017
  */
     int i,j,Nxr = Nxx-1;
     double ar[Nxr], br[Nxr], cr[Nxr], ur[Nxr], vr[Nxr], fur[Nxr], fvr[Nxr];
     double xfinal[Nxx];
     for(i=0;i<Nxr;i++)     br[i] = bb[i+1];
     for(i=0;i<(Nxr-1);i++) cr[i] = cc[i+1];
     cr[Nxr-1] = 0;
     ar[0] = 0;
     for(i=1;i<Nxr;i++)     ar[i] = aa[i+1];

     for(i=0;i<Nxr;i++)    fur[i] = ff[i+1];

     for(i=0;i<Nxr;i++)    fvr[i] = 0;
     fvr[0]   = -aa[1];
     fvr[Nxr-1] = -cc[Nxr];

     solve_tridiagonal_in_place_destructive(&fur[0], Nxr, &ar[0], &br[0], &cr[0]);
     solve_tridiagonal_in_place_destructive(&fvr[0], Nxr, &ar[0], &br[0], &cc[1]);
     xfinal[0] = (ff[0] - aa[0]*fur[Nxr-1] - cc[0]*fur[0])/(bb[0]+aa[0]*fvr[Nxr-1]+cc[0]*fvr[0]);
     ff[0] = xfinal[0];
     for(i=1;i<Nxx;i++){
        ff[i] = fur[i-1] + xfinal[0]*fvr[i-1];
     }
}
