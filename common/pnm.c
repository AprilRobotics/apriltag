/* $COPYRIGHT_UM
$LICENSE_BSD
*/

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "pnm.h"

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
            assert(pnm->format == PNM_FORMAT_GRAY || pnm->format == PNM_FORMAT_RGB || pnm->format == PNM_FORMAT_BINARY);
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
                assert(0);

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
                assert(0);

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
