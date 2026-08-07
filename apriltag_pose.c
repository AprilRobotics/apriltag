#include <math.h>
#include <stdio.h>

#include "common/debug_print.h"
#include "apriltag_pose.h"
#include "common/homography.h"
#include "common/svd33.h"


static inline void mat3_identity(double R[3][3])
{
    R[0][0] = 1; R[0][1] = 0; R[0][2] = 0;
    R[1][0] = 0; R[1][1] = 1; R[1][2] = 0;
    R[2][0] = 0; R[2][1] = 0; R[2][2] = 1;
}

static inline void mat3_sub(const double A[3][3], const double B[3][3], double C[3][3])
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            C[i][j] = A[i][j] - B[i][j];
}

// C = A*B. Safe even if C aliases A or B.
static inline void mat3_mul(const double A[3][3], const double B[3][3], double C[3][3])
{
    double T[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            double s = 0;
            for (int k = 0; k < 3; k++)
                s += A[i][k] * B[k][j];
            T[i][j] = s;
        }
    }
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            C[i][j] = T[i][j];
}

// C = A*B'. Safe even if C aliases A or B.
static inline void mat3_mul_transpose(const double A[3][3], const double B[3][3], double C[3][3])
{
    double T[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            double s = 0;
            for (int k = 0; k < 3; k++)
                s += A[i][k] * B[j][k];
            T[i][j] = s;
        }
    }
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            C[i][j] = T[i][j];
}

// y = A*x. y must not alias x.
static inline void mat3_mul_vec(const double A[3][3], const double x[3], double y[3])
{
    y[0] = A[0][0]*x[0] + A[0][1]*x[1] + A[0][2]*x[2];
    y[1] = A[1][0]*x[0] + A[1][1]*x[1] + A[1][2]*x[2];
    y[2] = A[2][0]*x[0] + A[2][1]*x[1] + A[2][2]*x[2];
}

// y = A'*x. y must not alias x.
static inline void mat3_mul_transpose_vec(const double A[3][3], const double x[3], double y[3])
{
    y[0] = A[0][0]*x[0] + A[1][0]*x[1] + A[2][0]*x[2];
    y[1] = A[0][1]*x[0] + A[1][1]*x[1] + A[2][1]*x[2];
    y[2] = A[0][2]*x[0] + A[1][2]*x[1] + A[2][2]*x[2];
}

// Determinant of a 3x3 matrix (matches common/matd.c matd_det).
static inline double mat3_det(const double A[3][3])
{
    return  A[0][0]*A[1][1]*A[2][2]
          - A[0][0]*A[1][2]*A[2][1]
          + A[0][1]*A[1][2]*A[2][0]
          - A[0][1]*A[1][0]*A[2][2]
          + A[0][2]*A[1][0]*A[2][1]
          - A[0][2]*A[1][1]*A[2][0];
}

// Inverse of a 3x3 matrix via the adjugate. Returns 0 (and leaves Inv
// untouched) if the matrix is exactly singular, matching matd_inverse.
static inline int mat3_inverse(const double A[3][3], double Inv[3][3])
{
    double det = mat3_det(A);
    if (det == 0)
        return 0;
    double invdet = 1.0 / det;
    Inv[0][0] = (A[1][1]*A[2][2] - A[1][2]*A[2][1]) * invdet;
    Inv[0][1] = (A[0][2]*A[2][1] - A[0][1]*A[2][2]) * invdet;
    Inv[0][2] = (A[0][1]*A[1][2] - A[0][2]*A[1][1]) * invdet;
    Inv[1][0] = (A[1][2]*A[2][0] - A[1][0]*A[2][2]) * invdet;
    Inv[1][1] = (A[0][0]*A[2][2] - A[0][2]*A[2][0]) * invdet;
    Inv[1][2] = (A[0][2]*A[1][0] - A[0][0]*A[1][2]) * invdet;
    Inv[2][0] = (A[1][0]*A[2][1] - A[1][1]*A[2][0]) * invdet;
    Inv[2][1] = (A[0][1]*A[2][0] - A[0][0]*A[2][1]) * invdet;
    Inv[2][2] = (A[0][0]*A[1][1] - A[0][1]*A[1][0]) * invdet;
    return 1;
}

static inline double vec3_dot(const double a[3], const double b[3])
{
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

static inline double vec3_norm(const double a[3])
{
    return sqrt(vec3_dot(a, a));
}

// r = a / |a|. Matches matd_vec_normalize (divides by magnitude). r may alias a.
static inline void vec3_normalize(const double a[3], double r[3])
{
    double mag = vec3_norm(a);
    r[0] = a[0] / mag;
    r[1] = a[1] / mag;
    r[2] = a[2] / mag;
}

// r = a x b. r must not alias a or b.
static inline void vec3_cross(const double a[3], const double b[3], double r[3])
{
    r[0] = a[1]*b[2] - a[2]*b[1];
    r[1] = a[2]*b[0] - a[0]*b[2];
    r[2] = a[0]*b[1] - a[1]*b[0];
}

/**
 * Projection operator from an image point: F = (v v') / (v' v).
 * Equivalent to the former calculate_F() but writes into a stack 3x3.
 */
static inline void mat3_calculate_F(const double v[3], double F[3][3])
{
    double inner = vec3_dot(v, v);
    double s = 1.0 / inner;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            F[i][j] = v[i] * v[j] * s;
}

/**
 * @param v Image points on the image plane.
 * @param p Object points in object space.
 * @outparam t Optimal translation.
 * @param R In/Outparam. Should be set to initial guess at R. Will be modified to be the optimal translation.
 * @param n_points Number of points.
 * @param n_steps Number of iterations.
 *
 * @return Object-space error after iteration.
 *
 * Implementation of Orthogonal Iteration from Lu, 2000.
 */
double orthogonal_iteration(matd_t** v, matd_t** p, matd_t** t, matd_t** R, int n_points, int n_steps) {
    // The pose model is always the tag's 4 corners; keep the scratch on the
    // stack so the iteration performs no heap allocation.
    assert(n_points <= 4);
    double P[4][3];    // object points
    double Pres[4][3]; // object points minus their mean
    double F[4][3][3]; // per-point projection operators
    double Q[4][3];    // per-step rotated points

    double p_mean[3] = {0, 0, 0};
    for (int i = 0; i < n_points; i++) {
        for (int k = 0; k < 3; k++) {
            P[i][k] = p[i]->data[k];
            p_mean[k] += P[i][k];
        }
    }
    for (int k = 0; k < 3; k++)
        p_mean[k] /= n_points;

    for (int i = 0; i < n_points; i++)
        for (int k = 0; k < 3; k++)
            Pres[i][k] = P[i][k] - p_mean[k];

    // Compute M1_inv = (I - mean(F))^-1.
    double avg_F[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
    for (int i = 0; i < n_points; i++) {
        double vi[3] = { v[i]->data[0], v[i]->data[1], v[i]->data[2] };
        mat3_calculate_F(vi, F[i]);
        for (int a = 0; a < 3; a++)
            for (int b = 0; b < 3; b++)
                avg_F[a][b] += F[i][a][b];
    }
    for (int a = 0; a < 3; a++)
        for (int b = 0; b < 3; b++)
            avg_F[a][b] /= n_points;

    double I3[3][3];
    mat3_identity(I3);
    double M1[3][3], M1_inv[3][3];
    mat3_sub(I3, avg_F, M1);
    mat3_inverse(M1, M1_inv);

    // Current rotation estimate (initialized from the supplied guess) and
    // translation (filled on the first step).
    double Rcur[3][3];
    for (int a = 0; a < 3; a++)
        for (int b = 0; b < 3; b++)
            Rcur[a][b] = MATD_EL(*R, a, b);
    double tcur[3] = {0, 0, 0};

    double prev_error = HUGE_VAL;
    // Iterate.
    for (int i = 0; i < n_steps; i++) {
        // Calculate translation.
        double M2[3] = {0, 0, 0};
        for (int j = 0; j < n_points; j++) {
            double FmI[3][3], tmp[3][3], upd[3];
            mat3_sub(F[j], I3, FmI);
            mat3_mul(FmI, Rcur, tmp);
            mat3_mul_vec(tmp, P[j], upd);
            M2[0] += upd[0]; M2[1] += upd[1]; M2[2] += upd[2];
        }
        for (int k = 0; k < 3; k++)
            M2[k] /= n_points;
        mat3_mul_vec(M1_inv, M2, tcur);

        // Calculate rotation.
        double q_mean[3] = {0, 0, 0};
        for (int j = 0; j < n_points; j++) {
            double Rp[3];
            mat3_mul_vec(Rcur, P[j], Rp);
            Rp[0] += tcur[0]; Rp[1] += tcur[1]; Rp[2] += tcur[2];
            mat3_mul_vec(F[j], Rp, Q[j]);
            q_mean[0] += Q[j][0]; q_mean[1] += Q[j][1]; q_mean[2] += Q[j][2];
        }
        for (int k = 0; k < 3; k++)
            q_mean[k] /= n_points;

        double M3[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
        for (int j = 0; j < n_points; j++) {
            double d[3] = { Q[j][0] - q_mean[0], Q[j][1] - q_mean[1], Q[j][2] - q_mean[2] };
            for (int a = 0; a < 3; a++)
                for (int b = 0; b < 3; b++)
                    M3[a][b] += d[a] * Pres[j][b];
        }

        // Rotation update: the closest rotation to M3 (orthogonal Procrustes).
        double U[9], S[3], Vm[9];
        svd33(&M3[0][0], U, S, Vm);
        // Rcur = U * V'
        for (int a = 0; a < 3; a++)
            for (int b = 0; b < 3; b++)
                Rcur[a][b] = U[a*3+0]*Vm[b*3+0] + U[a*3+1]*Vm[b*3+1] + U[a*3+2]*Vm[b*3+2];
        // Flip the last column if U*V' came out as a reflection (det < 0), so
        // Rcur is a proper rotation.
        if (mat3_det(Rcur) < 0) {
            Rcur[0][2] = -Rcur[0][2];
            Rcur[1][2] = -Rcur[1][2];
            Rcur[2][2] = -Rcur[2][2];
        }

        double error = 0;
        for (int j = 0; j < n_points; j++) {
            double Rp[3], err_vec[3], ImF[3][3];
            mat3_mul_vec(Rcur, P[j], Rp);
            Rp[0] += tcur[0]; Rp[1] += tcur[1]; Rp[2] += tcur[2];
            mat3_sub(I3, F[j], ImF);
            mat3_mul_vec(ImF, Rp, err_vec);
            error += vec3_dot(err_vec, err_vec);
        }
        prev_error = error;
    }

    // Write the results back into the supplied matd outputs.
    for (int a = 0; a < 3; a++)
        for (int b = 0; b < 3; b++)
            MATD_EL(*R, a, b) = Rcur[a][b];
    for (int k = 0; k < 3; k++)
        (*t)->data[k] = tcur[k];

    return prev_error;
}

/**
 * Evaluates polynomial p at x.
 */
double polyval(double* p, int degree, double x) {
    double ret = 0;
    for (int i = 0; i <= degree; i++) {
        ret += p[i]*pow(x, i);
    }
    return ret;
}

/**
 * Numerically solve small degree polynomials. This is a customized method. It
 * ignores roots larger than 1000 and only gives small roots approximately.
 *
 * @param p Array of parameters s.t. p(x) = p[0] + p[1]*x + ...
 * @param degree The degree of p(x).
 * @outparam roots
 * @outparam n_roots
 */
void solve_poly_approx(double* p, int degree, double* roots, int* n_roots) {
    static const int MAX_ROOT = 1000;
    if (degree == 1) {
        if (fabs(p[0]) > MAX_ROOT*fabs(p[1])) {
            *n_roots = 0;
        } else {
            roots[0] = -p[0]/p[1];
            *n_roots = 1;
        }
        return;
    }

    // Calculate roots of derivative. This is only ever called with degree <= 4,
    // so the scratch fits on the stack.
    assert(degree <= 4);
    double p_der[4];
    for (int i = 0; i < degree; i++) {
        p_der[i] = (i + 1) * p[i+1];
    }

    double der_roots[4];
    int n_der_roots;
    solve_poly_approx(p_der, degree - 1, der_roots, &n_der_roots);


    // Go through all possibilities for roots of the polynomial.
    *n_roots = 0;
    for (int i = 0; i <= n_der_roots; i++) {
        double min;
        if (i == 0) {
            min = -MAX_ROOT;
        } else {
            min = der_roots[i - 1];
        }

        double max;
        if (i == n_der_roots) {
            max = MAX_ROOT;
        } else {
            max = der_roots[i];
        }

        if (polyval(p, degree, min)*polyval(p, degree, max) < 0) {
            // We have a zero-crossing in this interval, use a combination of Newton' and bisection.
            // Some thanks to Numerical Recipes in C.

            double lower;
            double upper;
            if (polyval(p, degree, min) < polyval(p, degree, max)) {
                lower = min;
                upper = max;
            } else {
                lower = max;
                upper = min;
            }
            double root = 0.5*(lower + upper);
            double dx_old = upper - lower;
            double dx = dx_old;
            double f = polyval(p, degree, root);
            double df = polyval(p_der, degree - 1, root);

            for (int j = 0; j < 100; j++) {
                if (((f + df*(upper - root))*(f + df*(lower - root)) > 0)
                        || (fabs(2*f) > fabs(dx_old*df))) {
                    dx_old = dx;
                    dx = 0.5*(upper - lower);
                    root = lower + dx;
                } else {
                    dx_old = dx;
                    dx = -f/df;
                    root += dx;
                }

                if (root == upper || root == lower) {
                    break;
                }

                f = polyval(p, degree, root);
                df = polyval(p_der, degree - 1, root);

                if (f > 0) {
                    upper = root;
                } else {
                    lower = root;
                }
            }

            roots[(*n_roots)++] = root;
        } else if(polyval(p, degree, max) == 0) {
            // Double/triple root.
            roots[(*n_roots)++] = max;
        }
    }
}

/**
 * Given a local minima of the pose error tries to find the other minima.
 */
matd_t* fix_pose_ambiguities(matd_t** v, matd_t** p, matd_t* t, matd_t* R, int n_points) {
    double I3[3][3];
    mat3_identity(I3);

    // 1. Find R_t
    double t_vec[3] = { t->data[0], t->data[1], t->data[2] };
    double R_t_3[3];
    vec3_normalize(t_vec, R_t_3);

    double e_x[3] = {1, 0, 0};
    double dot_ex = vec3_dot(e_x, R_t_3);
    double R_t_1_tmp[3] = {
        e_x[0] - dot_ex*R_t_3[0],
        e_x[1] - dot_ex*R_t_3[1],
        e_x[2] - dot_ex*R_t_3[2] };
    double R_t_1[3];
    vec3_normalize(R_t_1_tmp, R_t_1);

    double R_t_2[3];
    vec3_cross(R_t_3, R_t_1, R_t_2);

    double R_t[3][3] = {
        { R_t_1[0], R_t_1[1], R_t_1[2] },
        { R_t_2[0], R_t_2[1], R_t_2[2] },
        { R_t_3[0], R_t_3[1], R_t_3[2] } };

    // 2. Find R_z
    double Rmat[3][3];
    for (int a = 0; a < 3; a++)
        for (int b = 0; b < 3; b++)
            Rmat[a][b] = MATD_EL(R, a, b);
    double R_1_prime[3][3];
    mat3_mul(R_t, Rmat, R_1_prime);

    double r31 = R_1_prime[2][0];
    double r32 = R_1_prime[2][1];
    double hypotenuse = sqrt(r31*r31 + r32*r32);
    if (hypotenuse < 1e-100) {
        r31 = 1;
        r32 = 0;
        hypotenuse = 1;
    }
    double R_z[3][3] = {
        { r31/hypotenuse, -r32/hypotenuse, 0 },
        { r32/hypotenuse,  r31/hypotenuse, 0 },
        { 0, 0, 1 } };

    // 3. Calculate parameters of Eos
    double R_trans[3][3];
    mat3_mul(R_1_prime, R_z, R_trans);
    double sin_gamma = -R_trans[0][1];
    double cos_gamma = R_trans[1][1];
    double R_gamma[3][3] = {
        { cos_gamma, -sin_gamma, 0 },
        { sin_gamma,  cos_gamma, 0 },
        { 0, 0, 1 } };

    double sin_beta = -R_trans[2][0];
    double cos_beta = R_trans[2][2];
    double t_initial = atan2(sin_beta, cos_beta);

    assert(n_points <= 4);
    double p_trans[4][3];
    double F_trans[4][3][3];
    double avg_F_trans[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
    for (int i = 0; i < n_points; i++) {
        double pi[3] = { p[i]->data[0], p[i]->data[1], p[i]->data[2] };
        mat3_mul_transpose_vec(R_z, pi, p_trans[i]); // R_z' * p[i]

        double vi[3] = { v[i]->data[0], v[i]->data[1], v[i]->data[2] };
        double v_trans[3];
        mat3_mul_vec(R_t, vi, v_trans); // R_t * v[i]
        mat3_calculate_F(v_trans, F_trans[i]);

        for (int a = 0; a < 3; a++)
            for (int b = 0; b < 3; b++)
                avg_F_trans[a][b] += F_trans[i][a][b];
    }
    for (int a = 0; a < 3; a++)
        for (int b = 0; b < 3; b++)
            avg_F_trans[a][b] /= n_points;

    double ImAvg[3][3], G[3][3];
    mat3_sub(I3, avg_F_trans, ImAvg);
    mat3_inverse(ImAvg, G);
    for (int a = 0; a < 3; a++)
        for (int b = 0; b < 3; b++)
            G[a][b] /= n_points;

    double M1[3][3] = {
        { 0, 0, 2 },
        { 0, 0, 0 },
        { -2, 0, 0 } };
    double M2[3][3] = {
        { -1, 0, 0 },
        { 0, 1, 0 },
        { 0, 0, -1 } };

    double b0[3] = {0, 0, 0};
    double b1[3] = {0, 0, 0};
    double b2[3] = {0, 0, 0};
    for (int i = 0; i < n_points; i++) {
        double FmI[3][3];
        mat3_sub(F_trans[i], I3, FmI);

        // op_tmp1 = (F - I) * R_gamma * p_trans
        double tmpA[3][3], v1[3];
        mat3_mul(FmI, R_gamma, tmpA);
        mat3_mul_vec(tmpA, p_trans[i], v1);

        // op_tmp2 = (F - I) * R_gamma * M1 * p_trans
        double tmpB[3][3], v2[3];
        mat3_mul(tmpA, M1, tmpB);
        mat3_mul_vec(tmpB, p_trans[i], v2);

        // op_tmp3 = (F - I) * R_gamma * M2 * p_trans
        double tmpC[3][3], v3[3];
        mat3_mul(tmpA, M2, tmpC);
        mat3_mul_vec(tmpC, p_trans[i], v3);

        for (int k = 0; k < 3; k++) {
            b0[k] += v1[k];
            b1[k] += v2[k];
            b2[k] += v3[k];
        }
    }
    double b0_[3], b1_[3], b2_[3];
    mat3_mul_vec(G, b0, b0_);
    mat3_mul_vec(G, b1, b1_);
    mat3_mul_vec(G, b2, b2_);

    double a0 = 0;
    double a1 = 0;
    double a2 = 0;
    double a3 = 0;
    double a4 = 0;
    for (int i = 0; i < n_points; i++) {
        double ImF[3][3];
        mat3_sub(I3, F_trans[i], ImF);

        // c0 = (I - F) * (R_gamma * p_trans + b0_)
        double Rg_p[3], inner0[3], c0[3];
        mat3_mul_vec(R_gamma, p_trans[i], Rg_p);
        for (int k = 0; k < 3; k++) inner0[k] = Rg_p[k] + b0_[k];
        mat3_mul_vec(ImF, inner0, c0);

        // c1 = (I - F) * (R_gamma * M1 * p_trans + b1_)
        double RgM1[3][3], RgM1_p[3], inner1[3], c1[3];
        mat3_mul(R_gamma, M1, RgM1);
        mat3_mul_vec(RgM1, p_trans[i], RgM1_p);
        for (int k = 0; k < 3; k++) inner1[k] = RgM1_p[k] + b1_[k];
        mat3_mul_vec(ImF, inner1, c1);

        // c2 = (I - F) * (R_gamma * M2 * p_trans + b2_)
        double RgM2[3][3], RgM2_p[3], inner2[3], c2[3];
        mat3_mul(R_gamma, M2, RgM2);
        mat3_mul_vec(RgM2, p_trans[i], RgM2_p);
        for (int k = 0; k < 3; k++) inner2[k] = RgM2_p[k] + b2_[k];
        mat3_mul_vec(ImF, inner2, c2);

        a0 += vec3_dot(c0, c0);
        a1 += 2 * vec3_dot(c0, c1);
        a2 += vec3_dot(c1, c1) + 2 * vec3_dot(c0, c2);
        a3 += 2 * vec3_dot(c1, c2);
        a4 += vec3_dot(c2, c2);
    }

    // 4. Solve for minima of Eos.
    double p0 = a1;
    double p1 = 2*a2 - 4*a0;
    double p2 = 3*a3 - 3*a1;
    double p3 = 4*a4 - 2*a2;
    double p4 = -a3;

    double roots[4];
    int n_roots;
    solve_poly_approx((double []) {p0, p1, p2, p3, p4}, 4, roots, &n_roots);

    double minima[4];
    int n_minima = 0;
    for (int i = 0; i < n_roots; i++) {
        double t1 = roots[i];
        double t2 = t1*t1;
        double t3 = t1*t2;
        double t4 = t1*t3;
        double t5 = t1*t4;
        // Check extrema is a minima.
        if (a2 - 2*a0 + (3*a3 - 6*a1)*t1 + (6*a4 - 8*a2 + 10*a0)*t2 + (-8*a3 + 6*a1)*t3 + (-6*a4 + 3*a2)*t4 + a3*t5 >= 0) {
            // And that it corresponds to an angle different than the known minimum.
            double t_cur = 2*atan(roots[i]);
            // We only care about finding a second local minima which is qualitatively
            // different than the first.
            if (fabs(t_cur - t_initial) > 0.1) {
                minima[n_minima++] = roots[i];
            }
        }
    }

    // 5. Get poses for minima.
    matd_t* ret = NULL;
    if (n_minima == 1) {
        double t_cur = minima[0];
        // R_beta = ((M2 * t_cur + M1) * t_cur + I) / (1 + t_cur^2)
        double R_beta[3][3];
        double scale = 1.0 / (1 + t_cur*t_cur);
        for (int a = 0; a < 3; a++)
            for (int b = 0; b < 3; b++)
                R_beta[a][b] = ((M2[a][b]*t_cur + M1[a][b])*t_cur + I3[a][b]) * scale;

        // ret = R_t' * R_gamma * R_beta * R_z'
        double tmp1[3][3], tmp2[3][3], result[3][3];
        double R_t_T[3][3];
        for (int a = 0; a < 3; a++)
            for (int b = 0; b < 3; b++)
                R_t_T[a][b] = R_t[b][a];
        mat3_mul(R_t_T, R_gamma, tmp1);   // R_t' * R_gamma
        mat3_mul(tmp1, R_beta, tmp2);     // * R_beta
        mat3_mul_transpose(tmp2, R_z, result); // * R_z'

        ret = matd_create(3, 3);
        for (int a = 0; a < 3; a++)
            for (int b = 0; b < 3; b++)
                MATD_EL(ret, a, b) = result[a][b];
    } else if (n_minima > 1)  {
        // This can happen if our prior pose estimate was not very good.
        debug_print("Error, more than one new minimum found.\n");
    }
    return ret;
}

/**
 * Estimate pose of the tag using the homography method.
 */
void estimate_pose_for_tag_homography(apriltag_detection_info_t* info, apriltag_pose_t* solution) {
    double scale = info->tagsize/2.0;

    matd_t *M_H = homography_to_pose(info->det->H, -info->fx, info->fy, info->cx, info->cy);
    MATD_EL(M_H, 0, 3) *= scale;
    MATD_EL(M_H, 1, 3) *= scale;
    MATD_EL(M_H, 2, 3) *= scale;

    matd_t* fix = matd_create(4, 4);
    MATD_EL(fix, 0, 0) = 1;
    MATD_EL(fix, 1, 1) = -1;
    MATD_EL(fix, 2, 2) = -1;
    MATD_EL(fix, 3, 3) = 1;

    matd_t* initial_pose = matd_multiply(fix, M_H);
    matd_destroy(M_H);
    matd_destroy(fix);

    solution->R = matd_create(3, 3);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            MATD_EL(solution->R, i, j) = MATD_EL(initial_pose, i, j);
        }
    }

    solution->t = matd_create(3, 1);
    for (int i = 0; i < 3; i++) {
        MATD_EL(solution->t, i, 0) = MATD_EL(initial_pose, i, 3);
    }
    matd_destroy(initial_pose);
}

/**
 * Estimate tag pose using orthogonal iteration.
 */
void estimate_tag_pose_orthogonal_iteration(
        apriltag_detection_info_t* info,
        double* err1,
        apriltag_pose_t* solution1,
        double* err2,
        apriltag_pose_t* solution2,
        int nIters) {
    double scale = info->tagsize/2.0;
    matd_t* p[4] = {
        matd_create_data(3, 1, (double[]) {-scale, scale, 0}),
        matd_create_data(3, 1, (double[]) {scale, scale, 0}),
        matd_create_data(3, 1, (double[]) {scale, -scale, 0}),
        matd_create_data(3, 1, (double[]) {-scale, -scale, 0})};
    matd_t* v[4];
    for (int i = 0; i < 4; i++) {
        v[i] = matd_create_data(3, 1, (double[]) {
                (info->det->p[i][0] - info->cx)/info->fx, (info->det->p[i][1] - info->cy)/info->fy, 1});
    }

    estimate_pose_for_tag_homography(info, solution1);
    *err1 = orthogonal_iteration(v, p, &solution1->t, &solution1->R, 4, nIters);
    solution2->R = fix_pose_ambiguities(v, p, solution1->t, solution1->R, 4);
    if (solution2->R) {
        solution2->t = matd_create(3, 1);
        *err2 = orthogonal_iteration(v, p, &solution2->t, &solution2->R, 4, nIters);
    } else {
        *err2 = HUGE_VAL;
    }

    for (int i = 0; i < 4; i++) {
        matd_destroy(p[i]);
        matd_destroy(v[i]);
    }
}

/**
 * Estimate tag pose.
 */
double estimate_tag_pose(apriltag_detection_info_t* info, apriltag_pose_t* pose) {
    double err1, err2;
    apriltag_pose_t pose1, pose2;
    estimate_tag_pose_orthogonal_iteration(info, &err1, &pose1, &err2, &pose2, 50);
    if (err1 <= err2) {
        pose->R = pose1.R;
        pose->t = pose1.t;
        if (pose2.R) {
            matd_destroy(pose2.t);
        }
        matd_destroy(pose2.R);
        return err1;
    } else {
        pose->R = pose2.R;
        pose->t = pose2.t;
        matd_destroy(pose1.R);
        matd_destroy(pose1.t);
        return err2;
    }
}
