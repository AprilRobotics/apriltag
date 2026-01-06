#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdarg.h>  // Added this header file to support variable arguments

#include "apriltag.h"
#include "apriltag_pose.h"
#include "common/matd.h"
#include "common/pjpeg.h"
#include "test/getline.h"

// Declare tag family creation functions - use tag36h11 instead of tagStandard41h12
apriltag_family_t *tag36h11_create(void);
void tag36h11_destroy(apriltag_family_t *tf);

// format function copied from test_detection.c
char* format(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int required = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    if (required < 0) {
        return NULL;
    }

    char *buffer = malloc(required + 1);
    if (!buffer) {
        return NULL;
    }

    va_start(args, fmt);
    int result = vsnprintf(buffer, required + 1, fmt, args);
    va_end(args);

    if (result < 0) {
        free(buffer);
        return NULL;
    }

    return buffer;
}

// Parse expected detection results
int parse_expected_detection(const char* line, int* id, double corners[4][2]) {
    char* line_copy = strdup(line);
    if (!line_copy) {
        return 0;
    }

    // Split ID and corners using comma
    char* token = strtok(line_copy, ",");
    if (!token) {
        free(line_copy);
        return 0;
    }

    *id = atoi(token);

    // Parse four corners
    for (int i = 0; i < 4; i++) {
        token = strtok(NULL, " ()");
        if (!token) {
            free(line_copy);
            return 0;
        }
        corners[i][0] = atof(token);

        token = strtok(NULL, " ()");
        if (!token) {
            free(line_copy);
            return 0;
        }
        corners[i][1] = atof(token);
    }

    free(line_copy);
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <image_path> <expected_data_path>\n", argv[0]);
        printf("Example: %s test/data/33369213973_9d9bb4cc96_c.jpg test/data/33369213973_9d9bb4cc96_c.txt\n", argv[0]);
        return 1;
    }

    const char *image_path = argv[1];
    const char *expected_data_path = argv[2];

    printf("Testing tag pose estimation with image: %s\n", image_path);
    printf("Expected data file: %s\n", expected_data_path);

    // Create AprilTag detector - use tag36h11 family
    apriltag_detector_t *td = apriltag_detector_create();
    apriltag_family_t *tf = tag36h11_create();
    apriltag_detector_add_family(td, tf);

    // Set detector parameters - use same parameters as test_detection.c
    td->quad_decimate = 1.0;  // Consistent with test_detection.c
    td->quad_sigma = 0.0;
    td->nthreads = 4;
    td->debug = 0;
    td->refine_edges = 0;  // Consistent with test_detection.c
    // Remove non-existent parameters
    // td->refine_decode = 0;
    // td->refine_pose = 0;

    // Load image
    int pjpeg_error;
    pjpeg_t *pjpeg = pjpeg_create_from_file(image_path, 0, &pjpeg_error);  // Fix function call, add error parameter
    if (!pjpeg || pjpeg_error) {
        printf("Failed to load image: %s, error code: %d\n", image_path, pjpeg_error);
        return 1;
    }

    image_u8_t *im = pjpeg_to_u8_baseline(pjpeg);
    pjpeg_destroy(pjpeg);

    if (!im) {
        printf("Failed to decode image: %s\n", image_path);
        return 1;
    }

    // Perform detection
    zarray_t *detections = apriltag_detector_detect(td, im);

    // Read expected data file
    FILE *expected_file = fopen(expected_data_path, "r");
    if (!expected_file) {
        printf("Failed to open expected data file: %s\n", expected_data_path);
        image_u8_destroy(im);
        apriltag_detections_destroy(detections);
        apriltag_detector_destroy(td);
        tag36h11_destroy(tf);
        return 1;
    }

    printf("Found %d detections in image\n", zarray_size(detections));

    // Perform pose estimation test for each detection
    int num_detections = zarray_size(detections);
    int pose_estimation_success = 0;

    for (int i = 0; i < num_detections; i++) {
        apriltag_detection_t *det;
        zarray_get(detections, i, &det);

        printf("Processing detection %d: tag ID %d\n", i, det->id);

        // Set camera parameters and tag size
        apriltag_detection_info_t info;
        info.det = det;
        info.tagsize = 0.16; // Assume tag size is 16cm
        info.fx = 600.0;     // Assume focal length
        info.fy = 600.0;
        info.cx = im->width / 2.0;  // Assume optical center is at image center
        info.cy = im->height / 2.0;

        // Perform pose estimation
        apriltag_pose_t pose;
        double err = estimate_tag_pose(&info, &pose);

        printf("  Estimated pose for tag %d:\n", det->id);
        printf("    Translation: [%f, %f, %f]\n", 
               MATD_EL(pose.t, 0, 0), 
               MATD_EL(pose.t, 1, 0), 
               MATD_EL(pose.t, 2, 0));
        printf("    Rotation matrix:\n");
        printf("      [%f, %f, %f]\n", 
               MATD_EL(pose.R, 0, 0), MATD_EL(pose.R, 0, 1), MATD_EL(pose.R, 0, 2));
        printf("      [%f, %f, %f]\n", 
               MATD_EL(pose.R, 1, 0), MATD_EL(pose.R, 1, 1), MATD_EL(pose.R, 1, 2));
        printf("      [%f, %f, %f]\n", 
               MATD_EL(pose.R, 2, 0), MATD_EL(pose.R, 2, 1), MATD_EL(pose.R, 2, 2));
        printf("    Reprojection error: %f\n", err);

        // Check if pose estimation was successful
        if (pose.R != NULL && pose.t != NULL && err < 1.0) { // Assume error less than 1.0 means success
            pose_estimation_success++;
        }

        // Free pose memory
        if (pose.R) matd_destroy(pose.R);
        if (pose.t) matd_destroy(pose.t);
    }

    // Output test results
    printf("\nPose estimation test results:\n");
    printf("  Successful pose estimations: %d/%d\n", pose_estimation_success, num_detections);

    if (pose_estimation_success == num_detections && num_detections > 0) {
        printf("  All pose estimations successful!\n");
    } else if (num_detections > 0) {
        printf("  Some pose estimations failed.\n");
    } else {
        printf("  No detections found in image.\n");
    }

    // Clean up resources
    image_u8_destroy(im);
    apriltag_detections_destroy(detections);
    apriltag_detector_destroy(td);
    tag36h11_destroy(tf);

    return (pose_estimation_success == num_detections && num_detections > 0) ? 0 : 1;
}
