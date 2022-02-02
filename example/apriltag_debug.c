
static void output_detections()
{
}

static void output_quads_ps(const struct quad *quads, uint32_t size) {
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

static void output_detections_pnm(const apriltag_detection_t *detections, uint32_t count)
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

static void output_detections_ps(const apriltag_detection_t *detections, uint32_t count)
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

void output_quads_raw(const quad *quads, uint32_t count)
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

void output_quads_fixed(const quad *quads, uint32_t count)
{
    if (td->debug) {
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
}

void output_decode_samples(const image_u8_t *im_samples)
{
    image_u8_write_pnm(im_samples, "debug_samples.pnm");
}
