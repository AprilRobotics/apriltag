/**
 * @file image_u8_parallel.c
 * @author MqCreaple (gmq14159@gmail.com)
 * @brief Parallelized processing of various image_u8 related functions.
 * @version 0.1
 * @date 2025-08-07
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "common/image_u8_parallel.h"
#include "common/workerpool.h"
#include "common/math_util.h"

static void convolve(const uint8_t *x, uint8_t *y, int sz, const uint8_t *k, int ksz)
{
    assert((ksz&1)==1);

    for (int i = 0; i < ksz/2 && i < sz; i++)
        y[i] = x[i];

    for (int i = 0; i < sz - ksz + 1; i++) {
        uint32_t acc = 0;

        for (int j = 0; j < ksz; j++)
            acc += k[j]*x[i+j];

        y[ksz/2 + i] = acc >> 8;
    }

    for (int i = sz - ksz/2; i < sz; i++)
        y[i] = x[i];
}

struct image_u8_convolve_2D_task {
    image_u8_t *im;
    const uint8_t *k;
    int ksz;
    int idx_st;
    int idx_ed;
};

void _image_u8_convolve_2D_thread_1(void *p) {
    struct image_u8_convolve_2D_task *params = (struct image_u8_convolve_2D_task*) p;
    image_u8_t *im = params->im;
    const uint8_t *k = params->k;
    int ksz = params->ksz;
    int y_st = params->idx_st;
    int y_ed = params->idx_ed;

    assert((ksz & 1) == 1); // ksz must be odd.

    uint8_t *x = malloc(sizeof(uint8_t)*im->stride);
    for (int y = y_st; y < y_ed; y++) {
        memcpy(x, &im->buf[y*im->stride], im->stride);
        convolve(x, &im->buf[y*im->stride], im->width, k, ksz);
    }
    free(x);
}

void _image_u8_convolve_2D_thread_2(void *p) {
    struct image_u8_convolve_2D_task *params = (struct image_u8_convolve_2D_task*) p;
    image_u8_t *im = params->im;
    const uint8_t *k = params->k;
    int ksz = params->ksz;
    int x_st = params->idx_st;
    int x_ed = params->idx_ed;

    uint8_t *xb = malloc(sizeof(uint8_t)*im->height);
    uint8_t *yb = malloc(sizeof(uint8_t)*im->height);
    for (int x = x_st; x < x_ed; x++) {

        for (int y = 0; y < im->height; y++)
            xb[y] = im->buf[y*im->stride + x];

        convolve(xb, yb, im->height, k, ksz);

        for (int y = 0; y < im->height; y++)
            im->buf[y*im->stride + x] = yb[y];
    }
    free(xb);
    free(yb);
}

void image_u8_convolve_2D_parallel(workerpool_t *wp, image_u8_t *im, const uint8_t *k, int ksz) {
    if(im->width * im->height < 65536) {
        // for small images, directly use single threaded convolution
        image_u8_convolve_2D(im, k, ksz);
        return;
    }
    int nthreads = workerpool_get_nthreads(wp);

    struct image_u8_convolve_2D_task *params = malloc(sizeof(struct image_u8_convolve_2D_task) * nthreads);
    int y_inc = im->height / nthreads;
    int y_remainder = im->height % nthreads;
    int last_y = 0;
    for(int idx = 0; idx < nthreads; idx++) {
        params[idx].im = im;
        params[idx].k = k;
        params[idx].ksz = ksz;
        params[idx].idx_st = last_y;
        last_y += y_inc;
        if(idx < y_remainder) {
            last_y += 1;     // distribute the remainders across the n threads
        }
        params[idx].idx_ed = last_y;
        workerpool_add_task(wp, _image_u8_convolve_2D_thread_1, &params[idx]);
    }
    workerpool_run(wp);

    int x_inc = im->width / nthreads;
    int x_remainder = im->width % nthreads;
    int last_x = 0;
    for(int idx = 0; idx < nthreads; idx++) {
        params[idx].im = im;
        params[idx].k = k;
        params[idx].ksz = ksz;
        params[idx].idx_st = last_x;
        last_x += x_inc;
        if(idx < x_remainder) {
            last_x += 1;     // distribute the remainders across the n threads
        }
        params[idx].idx_ed = last_x;
        workerpool_add_task(wp, _image_u8_convolve_2D_thread_2, &params[idx]);
    }
    workerpool_run(wp);

    free(params);
}

void image_u8_gaussian_blur_parallel(workerpool_t *wp, image_u8_t *im, double sigma, int ksz) {
    if (sigma == 0)
        return;

    assert((ksz & 1) == 1); // ksz must be odd.

    // build the kernel.
    double *dk = malloc(sizeof(double)*ksz);

    // for kernel of length 5:
    // dk[0] = f(-2), dk[1] = f(-1), dk[2] = f(0), dk[3] = f(1), dk[4] = f(2)
    for (int i = 0; i < ksz; i++) {
        int x = -ksz/2 + i;
        double v = exp(-.5*sq(x / sigma));
        dk[i] = v;
    }

    // normalize
    double acc = 0;
    for (int i = 0; i < ksz; i++)
        acc += dk[i];

    for (int i = 0; i < ksz; i++)
        dk[i] /= acc;

    uint8_t *k = malloc(sizeof(uint8_t)*ksz);
    for (int i = 0; i < ksz; i++)
        k[i] = dk[i]*255;

    free(dk);

    image_u8_convolve_2D_parallel(wp, im, k, ksz);
    free(k);
}
