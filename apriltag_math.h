/* $COPYRIGHT_UM
$LICENSE_BSD
*/

#pragma once

#include <math.h>

// Computes the cholesky factorization of A, putting the lower
// triangular matrix into R.
static inline void mat33_chol(const double *A,
                              double *R)
{
    // A[0] = R[0]*R[0]
    R[0] = sqrt(A[0]);

    // A[1] = R[0]*R[3];
    R[3] = A[1] / R[0];

    // A[2] = R[0]*R[6];
    R[6] = A[2] / R[0];

    // A[4] = R[3]*R[3] + R[4]*R[4]
    R[4] = sqrt(A[4] - R[3]*R[3]);

    // A[5] = R[3]*R[6] + R[4]*R[7]
    R[7] = (A[5] - R[3]*R[6]) / R[4];

    // A[8] = R[6]*R[6] + R[7]*R[7] + R[8]*R[8]
    R[8] = sqrt(A[8] - R[6]*R[6] - R[7]*R[7]);

    R[1] = 0;
    R[2] = 0;
    R[5] = 0;
}

static inline void mat33_lower_tri_inv(const double *A,
                                       double *R)
{
    // A[0]*R[0] = 1
    R[0] = 1 / A[0];

    // A[3]*R[0] + A[4]*R[3] = 0
    R[3] = -A[3]*R[0] / A[4];

    // A[4]*R[4] = 1
    R[4] = 1 / A[4];

    // A[6]*R[0] + A[7]*R[3] + A[8]*R[6] = 0
    R[6] = (-A[6]*R[0] - A[7]*R[3]) / A[8];

    // A[7]*R[4] + A[8]*R[7] = 0
    R[7] = -A[7]*R[4] / A[8];

    // A[8]*R[8] = 1
    R[8] = 1 / A[8];
}


static inline void mat33_sym_solve(const double *A,
                                   const double *B,
                                   double *R)
{
    double L[9];
    mat33_chol(A, L);

    double M[9];
    mat33_lower_tri_inv(L, M);

    double tmp[3];
    tmp[0] = M[0]*B[0];
    tmp[1] = M[3]*B[0] + M[4]*B[1];
    tmp[2] = M[6]*B[0] + M[7]*B[1] + M[8]*B[2];

    R[0] = M[0]*tmp[0] + M[3]*tmp[1] + M[6]*tmp[2];
    R[1] = M[4]*tmp[1] + M[7]*tmp[2];
    R[2] = M[8]*tmp[2];
}
