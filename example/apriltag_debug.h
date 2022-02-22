#pragma once

void output_preprocess_image(const image_u8_t *quad_im);
void output_quads_raw(const image_u8_t *im_orig, const zarray_t *quads);
void output_quads_fixed(const image_u8_t *im_orig, const zarray_t *quads);
void output_decode_samples(const image_u8_t *im_samples);
void output_threshold_pnm(const image_u8_t *im_thresh);
void output_segmentation(unionfind_t *uf, int w, int h, uint32_t min_cluster_pixels);
void output_clusters(const zarray_t *clusters, int w, int h);
void output_lines_ps(const image_u8_t *im_orig, const zarray_t *quads);
void output_quads_ps(const image_u8_t *im_orig, const zarray_t *quads);
void output_detections_all(const image_u8_t *im_orig, const zarray_t *detections);
void output_detections_pnm(const image_u8_t *im_orig, const zarray_t *detections);
void output_detections_ps(const image_u8_t *im_orig, const zarray_t *detections);


#define AT_SETUP_FILE_DIAG(diag) \
    (diag).preprocess = output_preprocess_image;        \
    (diag).quads_raw = output_quads_raw;                \
    (diag).quads_fixed = output_quads_fixed;            \
    (diag).samples = output_decode_samples;             \
    (diag).quads_threshold = output_threshold_pnm;      \
    (diag).segmentation = output_segmentation;          \
    (diag).clusters = output_clusters;                  \
    (diag).lines = output_lines_ps;                     \
    (diag).quads_final = output_quads_ps;               \
    (diag).detections = output_detections_all;

