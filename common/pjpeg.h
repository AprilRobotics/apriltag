/* $COPYRIGHT_UM
$LICENSE_LGPL
*/

#pragma once

#include "image_u8.h"
#include "image_u8x3.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pjpeg_component pjpeg_component_t;
struct pjpeg_component
{
    // resolution of this component (which is smaller than the
    // dimensions of the image if the channel has been sub-sampled.)
    uint32_t width, height;

    // number of bytes per row. May be larger than width for alignment
    // reasons.
    uint32_t stride;

    // data[y*stride + x]
    uint8_t *data;

    ////////////////////////////////////////////////////////////////
    // These items probably not of great interest to most
    // applications.
    uint8_t id; // the identifier associated with this component
    uint8_t hv; // horiz scale (high 4 bits) / vert scale (low 4 bits)
    uint8_t scalex, scaley; // derived from hv above
    uint8_t tq; // quantization table index

    // this filled in at the last moment by SOS
    uint8_t tda; // which huff tables will we use for DC (high 4 bits) and AC (low 4 bits)
};

typedef struct pjpeg pjpeg_t;
struct pjpeg
{
    // status of the decode is put here. Non-zero means error.
    int error;

    uint32_t width, height; // pixel dimensions

    int ncomponents;
    pjpeg_component_t *components;
};

enum PJPEG_FLAGS {
    PJPEG_STRICT = 1,  // Don't try to recover from errors.
    PJPEG_MJPEG = 2,   // Support JPGs with missing DHT segments.
};

enum PJPEG_ERROR {
    PJPEG_OKAY = 0,
    PJPEG_ERR_FILE, // something wrong reading file
    PJPEG_ERR_DQT, // something wrong with DQT marker
    PJPEG_ERR_SOF, // something wrong with SOF marker
    PJPEG_ERR_DHT, // something wrong with DHT marker
    PJPEG_ERR_SOS, // something wrong with SOS marker
    PJPEG_ERR_MISSING_DHT, // missing a necessary huffman table
    PJPEG_ERR_DRI, // something wrong with DRI marker
    PJPEG_ERR_RESET, // didn't get a reset marker where we expected. Corruption?
    PJPEG_ERR_EOF, // ran out of bytes while decoding
    PJEPG_ERR_UNSUPPORTED, // an unsupported format
};

pjpeg_t *pjpeg_create_from_file(const char *path, uint32_t flags, int *error);
pjpeg_t *pjpeg_create_from_buffer(uint8_t *buf, int buflen, uint32_t flags, int *error);
void pjpeg_destroy(pjpeg_t *pj);

image_u8_t *pjpeg_to_u8_baseline(pjpeg_t *pj);
image_u8x3_t *pjpeg_to_u8x3_baseline(pjpeg_t *pj);

#ifdef __cplusplus
}
#endif
