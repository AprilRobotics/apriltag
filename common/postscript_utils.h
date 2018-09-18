/* $COPYRIGHT_UM
$LICENSE_BSD
*/

#pragma once

// write commands in postscript language to render an image in the current
// graphics environment. The image will be rendered in one pixel per unit
// with Y up coordinate axis (e.g. upside down).
static void postscript_image(FILE *f, image_u8_t *im)
{
//    fprintf(f, "/readstring {\n  currentfile exch readhexstring pop\n} bind def\n");
    fprintf(f, "/picstr %d string def\n", im->width);

    fprintf(f, "%d %d 8 [1 0 0 1 0 0]\n",
            im->width, im->height);

    fprintf(f, "{currentfile picstr readhexstring pop}\nimage\n");

    for (int y = 0; y < im->height; y++) {
        for (int x = 0; x < im->width; x++) {
            uint8_t v = im->buf[y*im->stride + x];
            fprintf(f, "%02x", v);
            if ((x % 32)==31)
                fprintf(f, "\n");
        }
    }

    fprintf(f, "\n");
}
