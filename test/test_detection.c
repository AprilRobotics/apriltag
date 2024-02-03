#include <stdarg.h>
#include <apriltag.h>
#include <tag36h11.h>
#include <common/pjpeg.h>

#include "getline.h"


char*
format(const char* fmt, ...)
{
    va_list args1;
    va_list args2;

    va_start(args1, fmt);
    va_copy(args2, args1);

    const int N = vsnprintf(NULL, 0, fmt, args1);
    va_end(args1);

    if (N < 0) {
        va_end(args2);
        return NULL;
    }

    char* res_fmt = calloc(N+1, sizeof(char));
    const int n = vsnprintf(res_fmt, N+1, fmt, args2);
    va_end(args2);

    if (n != N) {
        free(res_fmt);
        return NULL;
    }

    // caller has to free returned string
    return res_fmt;
}

int
detection_corner_compare_function(const void *_a, const void *_b)
{
    apriltag_detection_t *a = *(apriltag_detection_t**) _a;
    apriltag_detection_t *b = *(apriltag_detection_t**) _b;

    return memcmp(a->p, b->p, sizeof(a));
}

int
main(int argc, char *argv[])
{
    if (argc!=2) {
        return EXIT_FAILURE;
    }

    // load image
    char* const path_img = format("%s.jpg", argv[1]);
    pjpeg_t *pjpeg = pjpeg_create_from_file(path_img, 0, NULL);
    image_u8_t *im = pjpeg_to_u8_baseline(pjpeg);
    free(path_img);

    // load true detection
    char* const path_det_true = format("%s.txt", argv[1]);
    FILE *fp = fopen(path_det_true, "r");
    if (fp == NULL) {
        return EXIT_FAILURE;
    }
    free(path_det_true);

    apriltag_detector_t *td = apriltag_detector_create();
    apriltag_family_t *tf = tag36h11_create();
    apriltag_detector_add_family(td, tf);

    const char fmt[] = "%i, %i, (%.4lf %.4lf), (%.4lf %.4lf), (%.4lf %.4lf), (%.4lf %.4lf)";

    bool ok = true;

    zarray_t *detections = apriltag_detector_detect(td, im);

    // sort detections by detected corners for deterministic sorting order
    zarray_sort(detections, detection_corner_compare_function);

    int i = 0;
    for (; i < zarray_size(detections); i++) {
        apriltag_detection_t *det;
        zarray_get(detections, i, &det);

        char* const det_fmt = format(fmt,
            i, det->id,
            det->p[0][0], det->p[0][1], det->p[1][0], det->p[1][1],
            det->p[2][0], det->p[2][1], det->p[3][0], det->p[3][1]);

        char* line = NULL;
        size_t len = 0;
        const ssize_t nread = getline(&line, &len, fp);
        if (nread == -1) {
            free(line);
            return EXIT_FAILURE;
        }

        printf("Got:      %s\n", det_fmt);
        printf("Expected: %s\n", line);

        // compare strings without the newline character (\n)
        if (strncmp(det_fmt, line, nread-1) != 0) {
            fprintf(stderr, "Mismatch.\nGot:\n  %s\nExpected:\n  %s\n", det_fmt, line);
            ok = false;
        }

        free(det_fmt);
        free(line);
    }

    // check that we compared the expected amount of detections
    // if there are no "true" detections left, we should be at the end of the file
    // if there are no "detected" detections left, we should be at the end of the array
    if ((fgetc(fp) != -1) && (i != zarray_size(detections))) {
        return EXIT_FAILURE;
    }

    fclose(fp);

    apriltag_detections_destroy(detections);
    image_u8_destroy(im);
    pjpeg_destroy(pjpeg);

    apriltag_detector_destroy(td);
    tag36h11_destroy(tf);

    if (!ok) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
