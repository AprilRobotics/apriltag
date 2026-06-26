/* Copyright (C) 2013-2016, The Regents of The University of Michigan.
All rights reserved.
This software was developed in the APRIL Robotics Lab under the
direction of Edwin Olson, ebolson@umich.edu. This software may be
available under alternative licensing terms; contact the address above.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the Regents of The University of Michigan.
*/

#include <math.h>

#include "svd22.h"
#include "svd33.h"

/** SVD 3x3.

    Stack-only specialization of matd_svd() for 3x3 inputs. It runs the same
    two-sided Jacobi sweep matd_svd() uses -- repeatedly zeroing the largest
    off-diagonal 2x2 block with the closed-form svd22() and accumulating the
    rotations -- but on fixed double[9] storage with no heap allocation. The
    Householder pre-reduction matd_svd() does first is omitted: it is an
    efficiency step for large matrices and unnecessary at 3x3.

    Element (r,c) of a 3x3 argument is stored at index r*3 + c (row major).
 **/
void svd33(const double A[9], double U[9], double S[3], double V[9])
{
    // B is driven toward diagonal form; LS and RS accumulate the left and
    // right rotations and become U and V respectively.
    double B[9];
    for (int i = 0; i < 9; i++)
        B[i] = A[i];

    double LS[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
    double RS[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

    // Converge to machine precision relative to the matrix scale. (matd_svd
    // uses a fixed absolute 1e-10, which under-converges for small-magnitude
    // inputs; a scale-relative tolerance keeps the result fully converged.)
    double anorm = 0;
    for (int i = 0; i < 9; i++)
        anorm += A[i] * A[i];
    anorm = sqrt(anorm);
    const double tol = 1e-15 * anorm;
    const int maxiters = 200 + 2 * 3 * 3;  // same iteration bound as matd_svd

    static const int P[3] = { 0, 0, 1 };
    static const int Q[3] = { 1, 2, 2 };

    for (int iter = 0; iter < maxiters; iter++) {
        int rotated = 0;
        for (int pair = 0; pair < 3; pair++) {
            int p = P[pair], q = Q[pair];
            if (fabs(B[p*3+q]) <= tol && fabs(B[q*3+p]) <= tol)
                continue;

            // Solve the 2x2 SVD of the [p,q] x [p,q] submatrix.
            // OPTIMIZATION OPPORTUNITY: svd22 derives this rotation with atan2/cos/sin;
            // a sqrt-only rotation would be several times faster here (this runs ~100x
            // per pose) at equal accuracy. Kept as svd22 for now for simplicity.
            double AQ[4] = { B[p*3+p], B[p*3+q], B[q*3+p], B[q*3+q] };
            double U2[4], S2[2], V2[4];
            svd22(AQ, U2, S2, V2);

            // B = U2' * B : mix rows p, q.
            for (int i = 0; i < 3; i++) {
                double vi = B[p*3+i], vj = B[q*3+i];
                B[p*3+i] = U2[0]*vi + U2[2]*vj;
                B[q*3+i] = U2[1]*vi + U2[3]*vj;
            }
            // B = B * V2 : mix cols p, q.
            for (int i = 0; i < 3; i++) {
                double vi = B[i*3+p], vj = B[i*3+q];
                B[i*3+p] = V2[0]*vi + V2[2]*vj;
                B[i*3+q] = V2[1]*vi + V2[3]*vj;
            }
            // LS = LS * U2 ; RS = RS * V2 : mix cols p, q.
            for (int i = 0; i < 3; i++) {
                double vi = LS[i*3+p], vj = LS[i*3+q];
                LS[i*3+p] = U2[0]*vi + U2[2]*vj;
                LS[i*3+q] = U2[1]*vi + U2[3]*vj;
            }
            for (int i = 0; i < 3; i++) {
                double vi = RS[i*3+p], vj = RS[i*3+q];
                RS[i*3+p] = V2[0]*vi + V2[2]*vj;
                RS[i*3+q] = V2[1]*vi + V2[3]*vj;
            }
            rotated = 1;
        }
        if (!rotated)
            break;
    }

    // The singular values are the diagonal of B. Fold the sign of any negative
    // one into the U side and sort descending -- same convention as matd_svd,
    // so the result satisfies A = U * diag(S) * V' with S >= 0.
    double sv[3] = { B[0], B[4], B[8] };
    for (int k = 0; k < 3; k++) {
        if (sv[k] < 0) {
            sv[k] = -sv[k];
            LS[0*3+k] = -LS[0*3+k];
            LS[1*3+k] = -LS[1*3+k];
            LS[2*3+k] = -LS[2*3+k];
        }
    }

    int idx[3] = { 0, 1, 2 };
    int changed;
    do {
        changed = 0;
        for (int i = 0; i + 1 < 3; i++) {
            if (sv[idx[i+1]] > sv[idx[i]]) {
                int t = idx[i]; idx[i] = idx[i+1]; idx[i+1] = t;
                changed = 1;
            }
        }
    } while (changed);

    for (int k = 0; k < 3; k++) {
        int c = idx[k];
        S[k] = sv[c];
        for (int r = 0; r < 3; r++) {
            U[r*3+k] = LS[r*3+c];
            V[r*3+k] = RS[r*3+c];
        }
    }
}
