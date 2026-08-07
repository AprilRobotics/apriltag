#include <stdio.h>
#include <stdlib.h>
#include <apriltag.h>
#include <tag36h11.h>
#include <common/pjpeg.h>

// This test verifies that the cached data inside the detector handle size changes

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <image.jpg>\n", argv[0]);
        return EXIT_FAILURE;
    }

    pjpeg_t *pjpeg = pjpeg_create_from_file(argv[1], 0, NULL);
    if (pjpeg == NULL) {
        fprintf(stderr, "Failed to load %s\n", argv[1]);
        return EXIT_FAILURE;
    }
    image_u8_t *im = pjpeg_to_u8_baseline(pjpeg);

    apriltag_family_t *tf = tag36h11_create();
    apriltag_detector_t *td = apriltag_detector_create();
    apriltag_detector_add_family(td, tf);
    td->refine_edges = false;

    // First detect with quad_decimate=2
    td->quad_decimate = 2;
    zarray_t *dets1 = apriltag_detector_detect(td, im);
    int n1 = zarray_size(dets1);
    printf("decimate=2: %d detections\n", n1);
    apriltag_detections_destroy(dets1);

    // Now detect with quad_decimate=1
    td->quad_decimate = 1;
    zarray_t *dets2 = apriltag_detector_detect(td, im);
    int n2 = zarray_size(dets2);
    printf("decimate=1: %d detections\n", n2);
    apriltag_detections_destroy(dets2);

    // Then detect with quad_decimate=2 again
    td->quad_decimate = 2;
    zarray_t *dets3 = apriltag_detector_detect(td, im);
    int n3 = zarray_size(dets3);
    printf("decimate=2: %d detections\n", n3);
    apriltag_detections_destroy(dets3);

    image_u8_destroy(im);
    pjpeg_destroy(pjpeg);
    apriltag_detector_destroy(td);
    tag36h11_destroy(tf);

    printf("PASS\n");
    return EXIT_SUCCESS;
}
