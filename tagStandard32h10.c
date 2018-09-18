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

#include <stdlib.h>
#include "apriltag.h"

apriltag_family_t *tagStandard32h10_create()
{
   apriltag_family_t *tf = calloc(1, sizeof(apriltag_family_t));
   tf->h = 10;
   tf->ncodes = 26;
   tf->codes = calloc(26, sizeof(uint64_t));
   tf->codes[0] = 0x000000003f784a6aUL;
   tf->codes[1] = 0x000000007a07502fUL;
   tf->codes[2] = 0x00000000b49655f4UL;
   tf->codes[3] = 0x00000000ef255bb9UL;
   tf->codes[4] = 0x0000000029b4617eUL;
   tf->codes[5] = 0x000000009ed26d08UL;
   tf->codes[6] = 0x00000000d96172cdUL;
   tf->codes[7] = 0x00000000890e841cUL;
   tf->codes[8] = 0x0000000038bb956bUL;
   tf->codes[9] = 0x00000000734a9b30UL;
   tf->codes[10] = 0x00000000e868a6baUL;
   tf->codes[11] = 0x0000000022f7ac7fUL;
   tf->codes[12] = 0x000000005d86b244UL;
   tf->codes[13] = 0x00000000d2a4bdceUL;
   tf->codes[14] = 0x000000000d33c393UL;
   tf->codes[15] = 0x000000008251cf1dUL;
   tf->codes[16] = 0x000000006c8de631UL;
   tf->codes[17] = 0x00000000a71cebf6UL;
   tf->codes[18] = 0x000000009159030aUL;
   tf->codes[19] = 0x0000000006770e94UL;
   tf->codes[20] = 0x00000000f0b325a8UL;
   tf->codes[21] = 0x000000002b422b6dUL;
   tf->codes[22] = 0x00000000157e4281UL;
   tf->codes[23] = 0x00000000c52b53d0UL;
   tf->codes[24] = 0x000000003a495f5aUL;
   tf->codes[25] = 0x0000000074d8651fUL;
   tf->nbits = 32;
   tf->bit_x = calloc(32, sizeof(uint32_t));
   tf->bit_y = calloc(32, sizeof(uint32_t));
   tf->bit_x[0] = -2;
   tf->bit_y[0] = -2;
   tf->bit_x[1] = -1;
   tf->bit_y[1] = -2;
   tf->bit_x[2] = 0;
   tf->bit_y[2] = -2;
   tf->bit_x[3] = 1;
   tf->bit_y[3] = -2;
   tf->bit_x[4] = 2;
   tf->bit_y[4] = -2;
   tf->bit_x[5] = 3;
   tf->bit_y[5] = -2;
   tf->bit_x[6] = 4;
   tf->bit_y[6] = -2;
   tf->bit_x[7] = 1;
   tf->bit_y[7] = 1;
   tf->bit_x[8] = 5;
   tf->bit_y[8] = -2;
   tf->bit_x[9] = 5;
   tf->bit_y[9] = -1;
   tf->bit_x[10] = 5;
   tf->bit_y[10] = 0;
   tf->bit_x[11] = 5;
   tf->bit_y[11] = 1;
   tf->bit_x[12] = 5;
   tf->bit_y[12] = 2;
   tf->bit_x[13] = 5;
   tf->bit_y[13] = 3;
   tf->bit_x[14] = 5;
   tf->bit_y[14] = 4;
   tf->bit_x[15] = 2;
   tf->bit_y[15] = 1;
   tf->bit_x[16] = 5;
   tf->bit_y[16] = 5;
   tf->bit_x[17] = 4;
   tf->bit_y[17] = 5;
   tf->bit_x[18] = 3;
   tf->bit_y[18] = 5;
   tf->bit_x[19] = 2;
   tf->bit_y[19] = 5;
   tf->bit_x[20] = 1;
   tf->bit_y[20] = 5;
   tf->bit_x[21] = 0;
   tf->bit_y[21] = 5;
   tf->bit_x[22] = -1;
   tf->bit_y[22] = 5;
   tf->bit_x[23] = 2;
   tf->bit_y[23] = 2;
   tf->bit_x[24] = -2;
   tf->bit_y[24] = 5;
   tf->bit_x[25] = -2;
   tf->bit_y[25] = 4;
   tf->bit_x[26] = -2;
   tf->bit_y[26] = 3;
   tf->bit_x[27] = -2;
   tf->bit_y[27] = 2;
   tf->bit_x[28] = -2;
   tf->bit_y[28] = 1;
   tf->bit_x[29] = -2;
   tf->bit_y[29] = 0;
   tf->bit_x[30] = -2;
   tf->bit_y[30] = -1;
   tf->bit_x[31] = 1;
   tf->bit_y[31] = 2;
   tf->width_at_border = 4;
   tf->total_width = 8;
   tf->reversed_border = false;
   return tf;
}

void tagStandard32h10_destroy(apriltag_family_t *tf)
{
   free(tf->codes);
   free(tf->bit_x);
   free(tf->bit_y);
   free(tf);
}
