/* $COPYRIGHT_UM
$LICENSE_BSD
*/

#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PNM_FORMAT_BINARY 4
#define PNM_FORMAT_GRAY 5
#define PNM_FORMAT_RGB  6

// supports ppm, pnm, pgm

typedef struct pnm pnm_t;
struct pnm
{
    int width, height;
    int format;
    int max; // 1 = binary, 255 = one byte, 65535 = two bytes

    uint32_t buflen;
    uint8_t *buf; // if max=65535, in big endian
};

pnm_t *pnm_create_from_file(const char *path);
void pnm_destroy(pnm_t *pnm);

#ifdef __cplusplus
}
#endif
