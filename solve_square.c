#include <math.h>
#include "apriltag_pose.h"

double solve_double_quadratic(
    double a1, double b1, double c1, double d1, double e1, double f1,
    double a2, double b2, double c2, double d2, double e2, double f2,
    double x0, double y0,
    double* pansx, double* pansy)
{
    double x = x0;
    double y = y0;
    double lastx = x;
    double lasty = y;
    double F1 = a1*x*x + b1*y*y + c1*x*y + d1*x + e1*y + f1;
    double F2 = a2*x*x + b2*y*y + c2*x*y + d2*x + e2*y + f2;

    double F1x, F1y, F2x, F2y, Fx, Fy, L, dx, dy, dF, rate;

    double threshold = 0.0000000001;
    int count = 0;

    while (! ((F1 >= -threshold) && (F1 <= threshold) &&
              (F2 >= -threshold) && (F2 <= threshold)    )
          )
    {
        // find the next (x,y) based on gradient of F1 and F2
        double stepx = 0;
        double stepy = 0;
        // F1
        Fx = 2*a1*x+c1*y+d1;
        Fy = 2*b1*y+c1*x+e1;
        L = 1/sqrt(Fx*Fx + Fy*Fy);
        dx = Fx * L;
        dy = Fy * L;
        dF = Fx * dx + Fy * dy;
        rate = (0-F1) / dF;
        stepx += rate * dx;
        stepy += rate * dy;
        // F2
        Fx = 2*a2*x+c2*y+d2;
        Fy = 2*b2*y+c2*x+e2;
        L = 1/sqrt(Fx*Fx + Fy*Fy);
        dx = Fx * L;
        dy = Fy * L;
        dF = Fx * dx + Fy * dy;
        rate = (0-F2) / dF;
        stepx += rate * dx;
        stepy += rate * dy;
        // check and add to (x,y)
        if ((x + stepx == lastx) && (y + stepy == lasty)) break;
        lastx = x;
        lasty = y;
        x += stepx;
        y += stepy;
        F1 = a1*x*x + b1*y*y + c1*x*y + d1*x + e1*y + f1;
        F2 = a2*x*x + b2*y*y + c2*x*y + d2*x + e2*y + f2;

        count++;
        if (count > 1000) break;
    }


    if ((F1 >= -threshold) && (F1 <= threshold) &&
        (F2 >= -threshold) && (F2 <= threshold)    )
    {
        *pansx = x; *pansy = y; return F1*F1 + F2*F2;
    }

    // if not finish, then it means (x,y) goes into an oscillation, use F=F1^2 + F2^2 to approach
    threshold *= threshold;
    double F = F1*F1 + F2*F2;

    while (F > threshold)
    {
        // find the next (x,y) based on gradient of F = F1^2 + F^2
        F1x = 2*a1*x+c1*y+d1;
        F1y = 2*b1*y+c1*x+e1;

        F2x = 2*a2*x+c2*y+d2;
        F2y = 2*b2*y+c2*x+e2;

        Fx = 2*F1 * F1x + 2*F2 * F2x;
        Fy = 2*F1 * F1y + 2*F2 * F2y;

        L = 1/sqrt(Fx*Fx + Fy*Fy);
        dx = Fx * L;
        dy = Fy * L;
        dF = Fx * dx + Fy * dy;
        rate = (0-F) / dF;
        x += rate * dx;
        y += rate * dy;

        F1 = a1*x*x + b1*y*y + c1*x*y + d1*x + e1*y + f1;
        F2 = a2*x*x + b2*y*y + c2*x*y + d2*x + e2*y + f2;
        F = F1*F1 + F2*F2;

        count++;
        if (count > 2000) break;
    }

    *pansx = x; *pansy = y;
    return F;
}

double solve_square(
    apriltag_detection_info_t* info,
    matd_t *xyz, matd_t *abc, matd_t *pqr)
{
    double u1, v1, u2, v2, u3, v3, u4, v4;
    double ku, kv, u0, v0;
    double L;

    u1 = info->det->p[0][0]; v1 = info->det->p[0][1];
    u2 = info->det->p[1][0]; v2 = info->det->p[1][1];
    u3 = info->det->p[2][0]; v3 = info->det->p[2][1];
    u4 = info->det->p[3][0]; v4 = info->det->p[3][1];
    ku = info->fx; kv = info->fy; u0 = info->cx; v0 = info->cy;
    L = info->tagsize;

    /*       image  u         camera / z
     *          ---------           /
     *        v |       |          ------------- x
     *          |       |          |
     *          ---------          | y
     *       tag
     *             abc
     *          ---------
     *          |   |   |       abc -> X
     *          |   ----|pqr    pqr -> Y 
     *          |  xyz  |       Z points inside
     *          ---------
     */

    // solve linar equation
    double c0 = -(u1*v2 - u2*v1 - u1*v3 + u3*v1 + u2*v4 - u4*v2 - u3*v4 + u4*v3)/
            (u1*v2 - u2*v1 - u1*v4 + u2*v3 - u3*v2 + u4*v1 + u3*v4 - u4*v3);
    double r0 = -(  u1*v3 - u3*v1 - u1*v4 - u2*v3 + u3*v2 + u4*v1 + u2*v4 - u4*v2)/
            (   u1*v2 - u2*v1 - u1*v4 + u2*v3 - u3*v2 + u4*v1 + u3*v4 - u4*v3);
        // got a c/z and r/z proximately, for quadric solving later as a starting point

    // a b p q x y c r z
    // 6 rows left
    double Ks[6] = {-0.5/ku,-0.5/kv,0.5/ku,0.5/kv,-0.5/ku,-0.5/kv};
    double Cs[6] = { u3+u2-u0-u0,   v3+v2-v0-v0,
                     u2-u1,         v2-v1,
                    -u3+u1,        -v3+v1
                  };
    double Rs[6] = { u3-u2,         v3-v2,
                    -u2-u1+u0+u0,  -v2-v1+v0+v0,
                    -u3+u1,        -v3+v1
                  };
    double Zs[6] = {-u3+u2,        -v3+v2,
                    u2-u1,         v2-v1,
                    u3+u1-u0-u0,   v3+v1-v0-v0,
                  };
    double ac,ar,az,bc,br,bz,xc,xr,xz;
    double pc,pr,pz,qc,qr,qz,yc,yr,yz;

    ac = -Cs[0]*Ks[0];  ar = -Rs[0]*Ks[0];  az = -Zs[0]*Ks[0];
    bc = -Cs[1]*Ks[1];  br = -Rs[1]*Ks[1];  bz = -Zs[1]*Ks[1];
    pc = -Cs[2]*Ks[2];  pr = -Rs[2]*Ks[2];  pz = -Zs[2]*Ks[2];
    qc = -Cs[3]*Ks[3];  qr = -Rs[3]*Ks[3];  qz = -Zs[3]*Ks[3];
    xc = -Cs[4]*Ks[4];  xr = -Rs[4]*Ks[4];  xz = -Zs[4]*Ks[4];
    yc = -Cs[5]*Ks[5];  yr = -Rs[5]*Ks[5];  yz = -Zs[5]*Ks[5];

    /* now we have the rate of x,y,a,b,p,q to c r z
     * solve double quadric equation next
     * we have two constrains:
     *      ap + bq + cr == 0 and
     *      a^2 + b^2 + c^2 == p^2 + q^2 + r^2 == hL^2
     * expressed by c r z, we have two equations like:
     *      Ac^2 + Br^2 + Ccr + Dcz + Erz + Fz^2 == 0
     * have c = cz * z,  r = rz * z, and eliminate z
     * we have two:
     *      A1c^2 + B1r^2 + C1cr + D1c + E1r + F1 == 0
     *      A2c^2 + B2r^2 + C2cr + D2c + E2r + F2 == 0
     */
    double a1, b1, c1, d1, e1, f1;
    double a2, b2, c2, d2, e2, f2;
    //c^2 ap            bq              cr
    a1 =  ac*pc         +bc*qc             ;
    //r^2
    b1 =  ar*pr         +br*qr             ;
    //cr
    c1 =  ac*pr+ar*pc   +bc*qr+br*qc    +1 ;
    //cz
    d1 =  ac*pz+az*pc   +bc*qz+bz*qc       ;
    //rz
    e1 =  ar*pz+az*pr   +br*qz+bz*qr       ;
    //z^2
    f1 =  az*pz         +bz*qz             ;

    double a3, b3, c3, d3, e3, f3;
    double a4, b4, c4, d4, e4, f4;
    //c^2 aa            bb              cc
    a3 = ac*ac          +bc*bc          +1 ;
    //r^2
    b3 = ar*ar          +br*br             ;
    //cr
    c3 = ac*ar+ar*ac    +bc*br+br*bc       ;
    //cz
    d3 = ac*az+az*ac    +bc*bz+bz*bc       ;
    //rz
    e3 = ar*az+az*ar    +br*bz+bz*br       ;
    //z^2
    f3 = az*az          +bz*bz             ;

    //c^2 pp            qq              rr
    a4 = pc*pc          +qc*qc             ;
    //r^2
    b4 = pr*pr          +qr*qr          +1 ;
    //cr
    c4 = pc*pr+pr*pc    +qc*qr+qr*qc       ;
    //cz
    d4 = pc*pz+pz*pc    +qc*qz+qz*qc       ;
    //rz
    e4 = pr*pz+pz*pr    +qr*qz+qz*qr       ;
    //z^2
    f4 = pz*pz          +qz*qz             ;

    a2 = a3-a4;
    b2 = b3-b4;
    c2 = c3-c4;
    d2 = d3-d4;
    e2 = e3-e4;
    f2 = f3-f4;

    double cz, rz;

    double ret = solve_double_quadratic(
        a1, b1, c1, d1, e1, f1,
        a2, b2, c2, d2, e2, f2,
        c0, r0,
        &cz, &rz);

    double a5,b5,c5,d5,e5,f5;
    //c^2
    a5 = a3 * cz * cz;
    //r^2
    b5 = b3 * rz * rz;
    //cr
    c5 = c3 * cz * rz;
    //cz
    d5 = d3 * cz;
    //rz
    e5 = e3 * rz;
    //z^2
    f5 = f3;

    double x,y,z,a,b,c,p,q,r;
    double hL = L / 2;
    z = sqrt(hL * hL / (a5+b5+c5+d5+e5+f5));
    c = cz * z;
    r = rz * z;
    x = xc * c + xr * r + xz * z;
    y = yc * c + yr * r + yz * z;
    a = ac * c + ar * r + az * z;
    b = bc * c + br * r + bz * z;
    p = pc * c + pr * r + pz * z;
    q = qc * c + qr * r + qz * z;

    // got x y z a b c p q r
    matd_put(xyz, 0, 0, x);
    matd_put(xyz, 1, 0, y);
    matd_put(xyz, 2, 0, z);
    matd_put(abc, 0, 0, a);
    matd_put(abc, 1, 0, b);
    matd_put(abc, 2, 0, c);
    matd_put(pqr, 0, 0, p);
    matd_put(pqr, 1, 0, q);
    matd_put(pqr, 2, 0, r);

    return ret;
}

matd_t* create_rotation_matrix(double th, matd_t *axis)
{
    double x = matd_get(axis, 0, 0);
    double y = matd_get(axis, 1, 0);
    double z = matd_get(axis, 2, 0);

    matd_t *r = matd_create(3, 3);

    matd_put(r, 0, 0, cos(th) + (1 - cos(th))*x*x);
    matd_put(r, 0, 1, (1 - cos(th))*x*y - sin(th)*z);
    matd_put(r, 0, 2, (1 - cos(th))*x*z + sin(th)*y);

    matd_put(r, 1, 0, (1 - cos(th))*y*x + sin(th)*z);
    matd_put(r, 1, 1, cos(th) + (1 - cos(th))*y*y);
    matd_put(r, 1, 2, (1 - cos(th))*y*z - sin(th)*x);

    matd_put(r, 2, 0, (1 - cos(th))*z*x - sin(th)*y);
    matd_put(r, 2, 1, (1 - cos(th))*z*y + sin(th)*x);
    matd_put(r, 2, 2, cos(th) + (1 - cos(th))*z*z);

    return r;
}

matd_t* create_square_rotation(matd_t *abc, matd_t *pqr)
{
    matd_t *X = matd_create(3, 1);
    matd_put(X, 0, 0, 1);
    matd_put(X, 1, 0, 0);
    matd_put(X, 2, 0, 0);
    matd_t *Y = matd_create(3, 1);
    matd_put(Y, 0, 0, 0);
    matd_put(Y, 1, 0, 1);
    matd_put(Y, 2, 0, 0);
    matd_scale_inplace(abc, 1/matd_vec_mag(abc));
    matd_scale_inplace(pqr, 1/matd_vec_mag(pqr));

    double th1 = acos(matd_vec_dot_product(X, abc));
    matd_t* axis1 = matd_crossproduct(X, abc);
    matd_t* R1; 
    if (matd_vec_mag(axis1) == 0)
        R1 = create_rotation_matrix(0, X);
    else {
        matd_scale_inplace(axis1, 1/matd_vec_mag(axis1));
        R1 = create_rotation_matrix(th1, axis1);
    }

    matd_t* Y_rotated = matd_multiply(R1, Y);

    double th2 = acos(matd_vec_dot_product(Y_rotated, pqr));
    matd_t* axis2 = matd_crossproduct(Y_rotated, pqr);
    matd_scale_inplace(axis2, 1/matd_vec_mag(axis2));
    if (matd_vec_dist(axis2, abc) > 1) matd_put(X, 0, 0, -1);
    matd_t *R2 = create_rotation_matrix(th2, X);

    matd_t *rotation = matd_multiply(R1, R2);

    matd_destroy(X);
    matd_destroy(Y);
    matd_destroy(axis1);
    matd_destroy(R1);
    matd_destroy(Y_rotated);
    matd_destroy(axis2);
    matd_destroy(R2);

    return rotation;
}

double estimate_tag_pose_solve_square(apriltag_detection_info_t* info, apriltag_pose_t* pose)
{
    pose->t = matd_create(3, 1);
    matd_t *abc = matd_create(3, 1);
    matd_t *pqr = matd_create(3, 1);
    double err = solve_square(info, pose->t, abc, pqr);
    pose->R = create_square_rotation(abc, pqr);
    matd_destroy(abc);
    matd_destroy(pqr);
    return err;
}
