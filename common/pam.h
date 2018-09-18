/* $COPYRIGHT_UM
$LICENSE_LGPL
*/

#pragma once

#include <stdint.h>

enum { PAM_GRAYSCALE_ALPHA = 5000, PAM_RGB_ALPHA, PAM_RGB, PAM_GRAYSCALE };

typedef struct pam pam_t;
struct pam
{
    int type; // one of PAM_*

    int width, height; // note, stride always width.
    int depth; // bytes per pixel
    int maxval; // maximum value per channel, e.g. 255 for 8bpp

    int datalen; // in bytes
    uint8_t *data;
};

pam_t *pam_create_from_file(const char *inpath);
int pam_write_file(pam_t *pam, const char *outpath);
void pam_destroy(pam_t *pam);

pam_t *pam_copy(pam_t *pam);

// NB doesn't handle many conversions yet.
pam_t *pam_convert(pam_t *in, int type);
