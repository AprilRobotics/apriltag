/* Copyright (C) 2013-2016, The Regents of The University of Michigan.
All rights reserved.
This software was developed in the APRIL Robotics Lab under the
direction of Edwin Olson, ebolson@umich.edu. This software may be
available under alternative licensing terms; contact the address above.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the Regents of The University of Michigan.
*/

#include "apriltag_config.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pnm.h"
#include "common/image_u8.h"
#include "common/diagnostic.h"

pnm_t *pnm_create_from_file(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (f == NULL)
        return NULL;

    pnm_t *pnm = calloc(1, sizeof(pnm_t));
    pnm->format = -1;

    char tmp[1024];
    int nparams = 0; // will be 3 when we're all done.
    int params[3];

    while (nparams < 3 && !(pnm->format == PNM_FORMAT_BINARY && nparams == 2)) {
        if (fgets(tmp, sizeof(tmp), f) == NULL)
            goto error;

        // skip comments
        if (tmp[0]=='#')
            continue;

        char *p = tmp;

        if (pnm->format == -1 && tmp[0]=='P') {
            pnm->format = tmp[1]-'0';
            AT_ASSERT(pnm->format == PNM_FORMAT_GRAY || pnm->format == PNM_FORMAT_RGB || pnm->format == PNM_FORMAT_BINARY);
            p = &tmp[2];
        }

        // pull integers out of this line until there are no more.
        while (nparams < 3 && *p!=0) {
            while (*p==' ')
                p++;

            // encounter rubbish? (End of line?)
            if (*p < '0' || *p > '9')
                break;

            int acc = 0;
            while (*p >= '0' && *p <= '9') {
                acc = acc*10 + *p - '0';
                p++;
            }

            params[nparams++] = acc;
            p++;
        }
    }

    pnm->width = params[0];
    pnm->height = params[1];
    pnm->max = params[2];

    switch (pnm->format) {
        case PNM_FORMAT_BINARY: {
            // files in the wild sometimes simply don't set max
            pnm->max = 1;

            pnm->buflen = pnm->height * ((pnm->width + 7)  / 8);
            pnm->buf = malloc(pnm->buflen);
            size_t len = fread(pnm->buf, 1, pnm->buflen, f);
            if (len != pnm->buflen)
                goto error;

            fclose(f);
            return pnm;
        }

        case PNM_FORMAT_GRAY: {
            if (pnm->max == 255)
                pnm->buflen = pnm->width * pnm->height;
            else if (pnm->max == 65535)
                pnm->buflen = 2 * pnm->width * pnm->height;
            else
                AT_ASSERT(0);

            pnm->buf = malloc(pnm->buflen);
            size_t len = fread(pnm->buf, 1, pnm->buflen, f);
            if (len != pnm->buflen)
                goto error;

            fclose(f);
            return pnm;
        }

        case PNM_FORMAT_RGB: {
            if (pnm->max == 255)
                pnm->buflen = pnm->width * pnm->height * 3;
            else if (pnm->max == 65535)
                pnm->buflen = 2 * pnm->width * pnm->height * 3;
            else
                AT_ASSERT(0);

            pnm->buf = malloc(pnm->buflen);
            size_t len = fread(pnm->buf, 1, pnm->buflen, f);
            if (len != pnm->buflen)
                goto error;
            fclose(f);
            return pnm;
        }
    }

error:
    fclose(f);

    if (pnm != NULL) {
        free(pnm->buf);
        free(pnm);
    }

    return NULL;
}

void pnm_destroy(pnm_t *pnm)
{
    if (pnm == NULL)
        return;

    free(pnm->buf);
    free(pnm);
}


////////////////////////////////////////////////////////////
// PNM file i/o
image_u8_t *image_u8_create_from_pnm(const char *path)
{
    return image_u8_create_from_pnm_alignment(path, DEFAULT_ALIGNMENT_U8);
}

image_u8_t *image_u8_create_from_pnm_alignment(const char *path, int alignment)
{
    pnm_t *pnm = pnm_create_from_file(path);
    if (pnm == NULL)
        return NULL;

    image_u8_t *im = NULL;

    switch (pnm->format) {
        case PNM_FORMAT_GRAY: {
            im = image_u8_create_alignment(pnm->width, pnm->height, alignment);

            if (pnm->max == 255) {
                for (int y = 0; y < im->height; y++)
                    memcpy(&im->buf[y*im->stride], &pnm->buf[y*im->width], im->width);
            } else if (pnm->max == 65535) {
                for (int y = 0; y < im->height; y++)
                    for (int x = 0; x < im->width; x++)
                        im->buf[y*im->stride + x] = pnm->buf[2*(y*im->width + x)];
            } else {
                AT_ASSERT_MSG(0, "invalid pnm->max");
            }

            break;
        }

        case PNM_FORMAT_RGB: {
            im = image_u8_create_alignment(pnm->width, pnm->height, alignment);

            if (pnm->max == 255) {
                // Gray conversion for RGB is gray = (r + g + g + b)/4
                for (int y = 0; y < im->height; y++) {
                    for (int x = 0; x < im->width; x++) {
                        uint8_t gray = (pnm->buf[y*im->width*3 + 3*x+0] +    // r
                                        pnm->buf[y*im->width*3 + 3*x+1] +    // g
                                        pnm->buf[y*im->width*3 + 3*x+1] +    // g
                                        pnm->buf[y*im->width*3 + 3*x+2])     // b
                                       / 4;

                        im->buf[y*im->stride + x] = gray;
                    }
                }
            } else if (pnm->max == 65535) {
                for (int y = 0; y < im->height; y++) {
                    for (int x = 0; x < im->width; x++) {
                        int r = pnm->buf[6*(y*im->width + x) + 0];
                        int g = pnm->buf[6*(y*im->width + x) + 2];
                        int b = pnm->buf[6*(y*im->width + x) + 4];

                        im->buf[y*im->stride + x] = (r + g + g + b) / 4;
                    }
                }
            } else {
                AT_ASSERT(0);
            }

            break;
        }

        case PNM_FORMAT_BINARY: {
            im = image_u8_create_alignment(pnm->width, pnm->height, alignment);

            // image is padded to be whole bytes on each row.

            // how many bytes per row on the input?
            int pbmstride = (im->width + 7) / 8;

            for (int y = 0; y < im->height; y++) {
                for (int x = 0; x < im->width; x++) {
                    int byteidx = y * pbmstride + x / 8;
                    int bitidx = 7 - (x & 7);

                    // ack, black is one according to pbm docs!
                    if ((pnm->buf[byteidx] >> bitidx) & 1)
                        im->buf[y*im->stride + x] = 0;
                    else
                        im->buf[y*im->stride + x] = 255;
                }
            }
            break;
        }
    }

    pnm_destroy(pnm);
    return im;
}
