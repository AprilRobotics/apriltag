/* $COPYRIGHT_UM
$LICENSE_LGPL
*/

#pragma once

#include <stdint.h>
#include "common/image_types.h"

#ifdef __cplusplus
extern "C" {
#endif


/////////////////////////////////////
// IMPORTANT NOTE ON BYTE ORDER
//
// Format conversion routines will (unless otherwise specified) assume
// R, G, B, A ordering of bytes.
//
/////////////////////////////////////

// Create or load an image. returns NULL on failure
image_u8x4_t *image_u8x4_create(unsigned int width, unsigned int height);
image_u8x4_t *image_u8x4_create_alignment(unsigned int width, unsigned int height, unsigned int alignment);
image_u8x4_t *image_u8x4_create_from_pnm(const char *path);

image_u8x4_t *image_u8x4_copy(const image_u8x4_t *in);

void image_u8x4_destroy(image_u8x4_t *im);

// Write a pnm. Return 0 on success.
// Currently supports GRAY and RGB
int image_u8x4_write_pnm(const image_u8x4_t *im, const char *path);

image_u8x4_t *image_u8x4_create_from_pam(const char *path);

    void image_u8x4_write_pam(const image_u8x4_t *im, const char *path);

#ifdef __cplusplus
}
#endif
