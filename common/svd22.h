/* $COPYRIGHT_UM
$LICENSE_BSD
*/

#pragma once

void svd22(const double A[4], double U[4], double S[2], double V[4]);

// for the matrix [a b; b d]
void svd_sym_singular_values(double A00, double A01, double A11,
                             double *Lmin, double *Lmax);
