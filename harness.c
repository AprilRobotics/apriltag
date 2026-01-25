#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "apriltag.h"
#include "tag36h11.h"
#include "common/image_u8.h"

int main(int argc, char **argv) {
    apriltag_detector_t *td = apriltag_detector_create();
    apriltag_family_t *tf = tag36h11_create();
    apriltag_detector_add_family(td, tf);

    td->quad_decimate = 1.0;
    td->quad_sigma = 0.0;
    td->nthreads = 1;
    td->debug = 0;
    td->refine_edges = 1;

    if (argc < 2) {
        printf("usage: %s file\n", argv[0]);
        return 1;
    }
    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        printf("Failed to open file\n");
        return 1;
    }
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    unsigned char *file_data = malloc(file_size);
    if (!file_data) {
        printf("Failed to allocate memory\n");
        fclose(f);
        return 1;
    }
    size_t bytes_read = fread(file_data, 1, file_size, f);
    if (bytes_read != file_size) {
        printf("Failed to read file\n");
        free(file_data);
        fclose(f);
        return 1;
    }

    fclose(f);

    unsigned char *buf = file_data; 
    
    do {
        int len = (int)file_size; 
        if (len < 5) continue;
        uint16_t width = (*(uint16_t *)buf) % 512;
        uint16_t height = (*(uint16_t *)(buf + 2)) % 512;
        if (width == 0) width = 1;
        if (height == 0) height = 1;
        int needed_pixels = width * height;
        int available_pixels = len - 4;
        if (available_pixels < needed_pixels) {
            height = available_pixels / width;
            if (height == 0) continue;
        }
        image_u8_t *im = image_u8_create(width, height);
        if (!im) continue;
        memcpy(im->buf, buf + 4, width * height);
        printf("Processing image of size %dx%d\n", width, height);
        td->debug = 1;
        zarray_t *detections = apriltag_detector_detect(td, im);
        apriltag_detections_destroy(detections);
        image_u8_destroy(im);
    } while (0);

    free(file_data);

    apriltag_detector_remove_family(td, tf);
    tag36h11_destroy(tf);
    apriltag_detector_destroy(td);

    return 0;
}