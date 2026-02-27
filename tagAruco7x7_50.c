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

/*
// https://github.com/opencv/opencv/blob/3e43d0cfca9753bcc4983f610b75d70c3f25f0cd/modules/objdetect/src/aruco/predefined_dictionaries.hpp
Copyright 2025 OpenCV

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <stdlib.h>
#include "tagAruco7x7_50.h"

static uint64_t codedata[50] = {
  0x0001baac5d5162e1UL,
  0x0001c8e27cd4152eUL,
  0x00013d145de29d73UL,
  0x00014b1b9832636fUL,
  0x00018dbfe4692816UL,
  0x0000b400b8763a44UL,
  0x0001a354fe10afbcUL,
  0x00007d2484df464bUL,
  0x0001100e227dcbbeUL,
  0x000173592cf9b4ebUL,
  0x0001e861290cc887UL,
  0x0000013a3c588d01UL,
  0x0000afa68d0c317fUL,
  0x0001395d039f0024UL,
  0x00017f88ffd665dcUL,
  0x0001989219ed0a4bUL,
  0x0001a7b216d788c6UL,
  0x00000744abb456fdUL,
  0x00004e75d9181f0cUL,
  0x000073ae5aca022eUL,
  0x000074b59b967411UL,
  0x00008edb92bcae44UL,
  0x000094c207d1df77UL,
  0x0000bbf812145973UL,
  0x0000ed6733178755UL,
  0x00012ae8d49b0c18UL,
  0x000130d13bc6d3e2UL,
  0x000146c8b01d76a0UL,
  0x000173f3dd0ca2d0UL,
  0x000195fc2a1b6ecfUL,
  0x0001a77e14cb72bdUL,
  0x0001e49a1d3e6c36UL,
  0x0001feeba1673de7UL,
  0x000072189774d725UL,
  0x0000c0b574f30a70UL,
  0x00001638d2c6ec7eUL,
  0x00001a702241e245UL,
  0x00002ced7821110bUL,
  0x000035638cb69586UL,
  0x000052d2122b1cf4UL,
  0x0000530664c7d8cdUL,
  0x00005fabab68ef0dUL,
  0x00006f07e4399ebfUL,
  0x00007bdf87ce9cd3UL,
  0x00008e879e03c863UL,
  0x0000926184ad1261UL,
  0x0000b89172408c9aUL,
  0x0000bdb1a7ba8950UL,
  0x0000dc92c446afacUL,
  0x0000e575f5c45142UL
};
apriltag_family_t *tagAruco7x7_50_create()
{
  apriltag_family_t *tf = calloc(1, sizeof(apriltag_family_t));
  tf->name = strdup("tagAruco7x7_50");
  tf->h = 19;
  tf->ncodes = 50;
  tf->codes = codedata;
  tf->nbits = 49;
  tf->bit_x = calloc(49, sizeof(uint32_t));
  tf->bit_y = calloc(49, sizeof(uint32_t));

  tf->bit_x[0] = 1;
  tf->bit_y[0] = 1;
  tf->bit_x[1] = 2;
  tf->bit_y[1] = 1;
  tf->bit_x[2] = 3;
  tf->bit_y[2] = 1;
  tf->bit_x[3] = 4;
  tf->bit_y[3] = 1;
  tf->bit_x[4] = 5;
  tf->bit_y[4] = 1;
  tf->bit_x[5] = 6;
  tf->bit_y[5] = 1;
  tf->bit_x[6] = 2;
  tf->bit_y[6] = 2;
  tf->bit_x[7] = 3;
  tf->bit_y[7] = 2;
  tf->bit_x[8] = 4;
  tf->bit_y[8] = 2;
  tf->bit_x[9] = 5;
  tf->bit_y[9] = 2;
  tf->bit_x[10] = 3;
  tf->bit_y[10] = 3;
  tf->bit_x[11] = 4;
  tf->bit_y[11] = 3;

  tf->bit_x[12] = 7;
  tf->bit_y[12] = 1;
  tf->bit_x[13] = 7;
  tf->bit_y[13] = 2;
  tf->bit_x[14] = 7;
  tf->bit_y[14] = 3;
  tf->bit_x[15] = 7;
  tf->bit_y[15] = 4;
  tf->bit_x[16] = 7;
  tf->bit_y[16] = 5;
  tf->bit_x[17] = 7;
  tf->bit_y[17] = 6;
  tf->bit_x[18] = 6;
  tf->bit_y[18] = 2;
  tf->bit_x[19] = 6;
  tf->bit_y[19] = 3;
  tf->bit_x[20] = 6;
  tf->bit_y[20] = 4;
  tf->bit_x[21] = 6;
  tf->bit_y[21] = 5;
  tf->bit_x[22] = 5;
  tf->bit_y[22] = 3;
  tf->bit_x[23] = 5;
  tf->bit_y[23] = 4;

  tf->bit_x[24] = 7;
  tf->bit_y[24] = 7;
  tf->bit_x[25] = 6;
  tf->bit_y[25] = 7;
  tf->bit_x[26] = 5;
  tf->bit_y[26] = 7;
  tf->bit_x[27] = 4;
  tf->bit_y[27] = 7;
  tf->bit_x[28] = 3;
  tf->bit_y[28] = 7;
  tf->bit_x[29] = 2;
  tf->bit_y[29] = 7;
  tf->bit_x[30] = 6;
  tf->bit_y[30] = 6;
  tf->bit_x[31] = 5;
  tf->bit_y[31] = 6;
  tf->bit_x[32] = 4;
  tf->bit_y[32] = 6;
  tf->bit_x[33] = 3;
  tf->bit_y[33] = 6;
  tf->bit_x[34] = 5;
  tf->bit_y[34] = 5;
  tf->bit_x[35] = 4;
  tf->bit_y[35] = 5;

  tf->bit_x[36] = 1;
  tf->bit_y[36] = 7;
  tf->bit_x[37] = 1;
  tf->bit_y[37] = 6;
  tf->bit_x[38] = 1;
  tf->bit_y[38] = 5;
  tf->bit_x[39] = 1;
  tf->bit_y[39] = 4;
  tf->bit_x[40] = 1;
  tf->bit_y[40] = 3;
  tf->bit_x[41] = 1;
  tf->bit_y[41] = 2;
  tf->bit_x[42] = 2;
  tf->bit_y[42] = 6;
  tf->bit_x[43] = 2;
  tf->bit_y[43] = 5;
  tf->bit_x[44] = 2;
  tf->bit_y[44] = 4;
  tf->bit_x[45] = 2;
  tf->bit_y[45] = 3;
  tf->bit_x[46] = 3;
  tf->bit_y[46] = 5;
  tf->bit_x[47] = 3;
  tf->bit_y[47] = 4;

  tf->bit_x[48] = 4;
  tf->bit_y[48] = 4;
  tf->width_at_border = 9;
  tf->total_width = 11;
  tf->reversed_border = false;
  return tf;
}

void tagAruco7x7_50_destroy(apriltag_family_t *tf)
{
  free(tf->bit_x);
  free(tf->bit_y);
  free(tf->name);
  free(tf);
}
