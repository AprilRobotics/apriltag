#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "apriltag.h"

#include "common/image_u8.h"
#include "common/zarray.h"
#include "common/unionfind.h"

#include "example/image_u8x3.h"
#include "example/image_u8x4.h"
#include "example/postscript_utils.h"
#include "example/apriltag_debug.h"

void output_quads_ps(const image_u8_t *im_orig, const zarray_t *quads)
{
    // deallocate
    FILE *f = fopen("debug_quads.ps", "w");
    fprintf(f, "%%!PS\n\n");

    image_u8_t *darker = image_u8_copy(im_orig);
    image_u8_darken(darker);
    image_u8_darken(darker);

    // assume letter, which is 612x792 points.
    double scale = fmin(612.0/darker->width, 792.0/darker->height);
    fprintf(f, "%f %f scale\n", scale, scale);
    fprintf(f, "0 %d translate\n", darker->height);
    fprintf(f, "1 -1 scale\n");

    postscript_image(f, darker);

    image_u8_destroy(darker);
    srandom(0);

    for (int i = 0; i < zarray_size(quads); i++) {
        struct quad *q;
        zarray_get_volatile(quads, i, &q);

        float rgb[3];
        int bias = 100;

        for (int j = 0; j < 3; j++) {
            rgb[j] = bias + (random() % (255-bias));
        }

        fprintf(f, "%f %f %f setrgbcolor\n", rgb[0]/255.0f, rgb[1]/255.0f, rgb[2]/255.0f);
        fprintf(f, "%f %f moveto %f %f lineto %f %f lineto %f %f lineto %f %f lineto stroke\n",
                q->p[0][0], q->p[0][1],
                q->p[1][0], q->p[1][1],
                q->p[2][0], q->p[2][1],
                q->p[3][0], q->p[3][1],
                q->p[0][0], q->p[0][1]);
    }

    fprintf(f, "showpage\n");
    fclose(f);
}

void output_detections_all(const image_u8_t *im_orig, const zarray_t *detections)
{
    output_detections_pnm(im_orig, detections);
    output_detections_ps(im_orig, detections);
}

void output_detections_pnm(const image_u8_t *im_orig, const zarray_t *detections)
{
    image_u8_t *darker = image_u8_copy(im_orig);
    image_u8_darken(darker);
    image_u8_darken(darker);

    image_u8x3_t *out = image_u8x3_create(darker->width, darker->height);
    for (int y = 0; y < im_orig->height; y++) {
        for (int x = 0; x < im_orig->width; x++) {
            out->buf[y*out->stride + 3*x + 0] = darker->buf[y*darker->stride + x];
            out->buf[y*out->stride + 3*x + 1] = darker->buf[y*darker->stride + x];
            out->buf[y*out->stride + 3*x + 2] = darker->buf[y*darker->stride + x];
        }
    }

    image_u8_destroy(darker);
    srandom(0);

    for (int i = 0; i < zarray_size(detections); i++) {
        apriltag_detection_t *det;
        zarray_get(detections, i, &det);

        float rgb[3];
        int bias = 100;

        for (int j = 0; j < 3; j++) {
            rgb[j] = bias + (random() % (255-bias));
        }

        for (int j = 0; j < 4; j++) {
            int k = (j + 1) & 3;
            image_u8x3_draw_line(out,
                                 det->p[j][0], det->p[j][1], det->p[k][0], det->p[k][1],
                                 (uint8_t[]) { rgb[0], rgb[1], rgb[2] },
                                 1);
        }
    }

    image_u8x3_write_pnm(out, "debug_output.pnm");
    image_u8x3_destroy(out);
}

void output_detections_ps(const image_u8_t *im_orig, const zarray_t *detections)
{
    image_u8_t *darker = image_u8_copy(im_orig);
    image_u8_darken(darker);
    image_u8_darken(darker);

    // assume letter, which is 612x792 points.
    FILE *f = fopen("debug_output.ps", "w");
    fprintf(f, "%%!PS\n\n");
    double scale = fmin(612.0/darker->width, 792.0/darker->height);
    fprintf(f, "%f %f scale\n", scale, scale);
    fprintf(f, "0 %d translate\n", darker->height);
    fprintf(f, "1 -1 scale\n");
    postscript_image(f, darker);

    image_u8_destroy(darker);

    srandom(0);
    for (int i = 0; i < zarray_size(detections); i++) {
        apriltag_detection_t *det;
        zarray_get(detections, i, &det);

        float rgb[3];
        int bias = 100;

        for (int j = 0; j < 3; j++) {
            rgb[j] = bias + (random() % (255-bias));
        }

        fprintf(f, "%f %f %f setrgbcolor\n", rgb[0]/255.0f, rgb[1]/255.0f, rgb[2]/255.0f);
        fprintf(f, "%f %f moveto %f %f lineto %f %f lineto %f %f lineto %f %f lineto stroke\n",
                det->p[0][0], det->p[0][1],
                det->p[1][0], det->p[1][1],
                det->p[2][0], det->p[2][1],
                det->p[3][0], det->p[3][1],
                det->p[0][0], det->p[0][1]);
    }

    fprintf(f, "showpage\n");
    fclose(f);
}

void output_preprocess_image(const image_u8_t *quad_im)
{
    AT_ASSERT(quad_im != NULL);
    image_u8_write_pnm(quad_im, "debug_preprocess.pnm");
}

void output_quads_raw(const image_u8_t *im_orig, const zarray_t *quads)
{
    image_u8_t *im_quads = image_u8_copy(im_orig);
    image_u8_darken(im_quads);
    image_u8_darken(im_quads);

    srandom(0);

    for (int i = 0; i < zarray_size(quads); i++) {
        struct quad *quad;
        zarray_get_volatile(quads, i, &quad);

        const int bias = 100;
        int color = bias + (random() % (255-bias));

        image_u8_draw_line(im_quads, quad->p[0][0], quad->p[0][1], quad->p[1][0], quad->p[1][1], color, 1);
        image_u8_draw_line(im_quads, quad->p[1][0], quad->p[1][1], quad->p[2][0], quad->p[2][1], color, 1);
        image_u8_draw_line(im_quads, quad->p[2][0], quad->p[2][1], quad->p[3][0], quad->p[3][1], color, 1);
        image_u8_draw_line(im_quads, quad->p[3][0], quad->p[3][1], quad->p[0][0], quad->p[0][1], color, 1);
    }

    image_u8_write_pnm(im_quads, "debug_quads_raw.pnm");
    image_u8_destroy(im_quads);
}

void output_quads_fixed(const image_u8_t *im_orig, const zarray_t *quads)
{
    image_u8_t *im_quads = image_u8_copy(im_orig);
    image_u8_darken(im_quads);
    image_u8_darken(im_quads);

    srandom(0);

    for (int i = 0; i < zarray_size(quads); i++) {
        struct quad *quad;
        zarray_get_volatile(quads, i, &quad);

        const int bias = 100;
        int color = bias + (random() % (255-bias));

        image_u8_draw_line(im_quads, quad->p[0][0], quad->p[0][1], quad->p[1][0], quad->p[1][1], color, 1);
        image_u8_draw_line(im_quads, quad->p[1][0], quad->p[1][1], quad->p[2][0], quad->p[2][1], color, 1);
        image_u8_draw_line(im_quads, quad->p[2][0], quad->p[2][1], quad->p[3][0], quad->p[3][1], color, 1);
        image_u8_draw_line(im_quads, quad->p[3][0], quad->p[3][1], quad->p[0][0], quad->p[0][1], color, 1);

    }

    image_u8_write_pnm(im_quads, "debug_quads_fixed.pnm");
    image_u8_destroy(im_quads);
}

void output_decode_samples(const image_u8_t *im_samples)
{
    image_u8_write_pnm(im_samples, "debug_samples.pnm");
}

void output_threshold_pnm(const image_u8_t *im_thresh)
{
    image_u8_write_pnm(im_thresh, "debug_threshold.pnm");
}

void output_segmentation(unionfind_t *uf, int w, int h, uint32_t min_cluster_pixels)
{
    // make segmentation image.
    image_u8x3_t *d = image_u8x3_create(w, h);

    uint32_t *colors = (uint32_t*) calloc(w*h, sizeof(*colors));

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            uint32_t v = unionfind_get_representative(uf, y*w+x);

            if (unionfind_get_set_size(uf, v) < min_cluster_pixels)
                continue;

            uint32_t color = colors[v];
            uint8_t r = color >> 16,
                    g = color >> 8,
                    b = color;

            if (color == 0) {
                const int bias = 50;
                r = bias + (random() % (200-bias));
                g = bias + (random() % (200-bias));
                b = bias + (random() % (200-bias));
                colors[v] = (r << 16) | (g << 8) | b;
            }

            d->buf[y*d->stride + 3*x + 0] = r;
            d->buf[y*d->stride + 3*x + 1] = g;
            d->buf[y*d->stride + 3*x + 2] = b;
        }
    }

    free(colors);

    image_u8x3_write_pnm(d, "debug_segmentation.pnm");
    image_u8x3_destroy(d);
}

void output_clusters(const zarray_t *clusters, int w, int h)
{
    image_u8x3_t *d = image_u8x3_create(w, h);

    for (int i = 0; i < zarray_size(clusters); i++) {
        zarray_t *cluster;
        zarray_get(clusters, i, &cluster);

        uint32_t r, g, b;
        const int bias = 50;
        r = bias + (random() % (200-bias));
        g = bias + (random() % (200-bias));
        b = bias + (random() % (200-bias));

        for (int j = 0; j < zarray_size(cluster); j++) {
            struct pt *p;
            zarray_get_volatile(cluster, j, &p);

            int x = p->x / 2;
            int y = p->y / 2;
            d->buf[y*d->stride + 3*x + 0] = r;
            d->buf[y*d->stride + 3*x + 1] = g;
            d->buf[y*d->stride + 3*x + 2] = b;
        }
    }

    image_u8x3_write_pnm(d, "debug_clusters.pnm");
    image_u8x3_destroy(d);
}


void output_lines_ps(const image_u8_t *im_orig, const zarray_t *quads)
{
    FILE *f = fopen("debug_lines.ps", "w");
    fprintf(f, "%%!PS\n\n");

    image_u8_t *im2 = image_u8_copy(im_orig);
    image_u8_darken(im2);
    image_u8_darken(im2);

    // assume letter, which is 612x792 points.
    double scale = fmin(612.0/im_orig->width, 792.0/im2->height);
    fprintf(f, "%.15f %.15f scale\n", scale, scale);
    fprintf(f, "0 %d translate\n", im2->height);
    fprintf(f, "1 -1 scale\n");

    postscript_image(f, im2);

    image_u8_destroy(im2);

    for (int i = 0; i < zarray_size(quads); i++) {
        const struct quad *q;
        zarray_get_volatile(quads, i, &q);

        float rgb[3];
        int bias = 100;

        for (int j = 0; j < 3; j++)
            rgb[j] = bias + (random() % (255-bias));

        fprintf(f, "%f %f %f setrgbcolor\n", rgb[0]/255.0f, rgb[1]/255.0f, rgb[2]/255.0f);
        fprintf(f, "%.15f %.15f moveto %.15f %.15f lineto %.15f %.15f lineto %.15f %.15f lineto %.15f %.15f lineto stroke\n",
                q->p[0][0], q->p[0][1],
                q->p[1][0], q->p[1][1],
                q->p[2][0], q->p[2][1],
                q->p[3][0], q->p[3][1],
                q->p[0][0], q->p[0][1]);
    }

    fclose(f);
}
