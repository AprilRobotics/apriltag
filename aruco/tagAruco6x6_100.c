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
#include "tagAruco6x6_100.h"

static uint64_t codedata[100] = {
    0x0000000187359537UL,
    0x00000000bebe30f0UL,
    0x00000001674ad6c5UL,
    0x0000000cca05f46cUL,
    0x0000000d0d1ea69eUL,
    0x0000000db17045e8UL,
    0x000000041072bb74UL,
    0x00000008a0ad7603UL,
    0x00000003136ef12eUL,
    0x000000038c24efe6UL,
    0x000000047fdf132aUL,
    0x00000004b3cba425UL,
    0x0000000741459c8fUL,
    0x00000008371b88ffUL,
    0x00000008def1bd41UL,
    0x0000000a22f1f2bdUL,
    0x00000000f26c9aa7UL,
    0x000000015497da67UL,
    0x0000000308c11164UL,
    0x0000000489def7c3UL,
    0x000000053e85b9beUL,
    0x000000062448cc7cUL,
    0x0000000735ba1c39UL,
    0x000000099bd7319dUL,
    0x0000000af8148320UL,
    0x0000000c17a4301bUL,
    0x0000000c7a520881UL,
    0x0000000e54034939UL,
    0x0000000e897c40b3UL,
    0x0000000ed8be3f4cUL,
    0x0000000f8ba3d752UL,
    0x000000001f9eff7dUL,
    0x0000000055ddb604UL,
    0x00000000be25174aUL,
    0x0000000142fb59f0UL,
    0x0000000164f8a5acUL,
    0x00000003910df6ddUL,
    0x00000004c75c1458UL,
    0x00000005401c0bfeUL,
    0x00000005ab1ca5edUL,
    0x00000006051744e1UL,
    0x000000061b0e30c5UL,
    0x00000006520d7ba0UL,
    0x00000006fb6769ffUL,
    0x00000007207fc6a6UL,
    0x0000000765896e4bUL,
    0x00000007a2c4f517UL,
    0x00000008050d5374UL,
    0x000000088127e3baUL,
    0x000000097924acf5UL,
    0x00000009a054b06bUL,
    0x00000009bec85c73UL,
    0x0000000a45347d68UL,
    0x0000000b6e57e84dUL,
    0x0000000b7773c770UL,
    0x0000000b8569e404UL,
    0x0000000c10da8f0aUL,
    0x0000000c3c466640UL,
    0x0000000c64aed8adUL,
    0x0000000c9f3ccfd0UL,
    0x0000000cc4592762UL,
    0x0000000cd9c6c20dUL,
    0x0000000cee30adbaUL,
    0x0000000e56e7a3a6UL,
    0x0000000efcd25392UL,
    0x0000000f5ff92d7eUL,
    0x00000002b4e8a647UL,
    0x00000002f39b337fUL,
    0x000000037dd57e97UL,
    0x0000000a6923c68cUL,
    0x0000000ae992bd80UL,
    0x0000000d8b844ca1UL,
    0x0000000052819cf9UL,
    0x0000000074c0f91bUL,
    0x00000000fd6b6b50UL,
    0x00000001144a026cUL,
    0x0000000121afbfafUL,
    0x000000012893eb1cUL,
    0x0000000134e4c0d4UL,
    0x000000019ba03529UL,
    0x00000001b0392a30UL,
    0x00000001ca1e1341UL,
    0x00000001d8b7a6d6UL,
    0x0000000276934eaeUL,
    0x00000002a1c5ade8UL,
    0x000000035201571eUL,
    0x000000037147bb41UL,
    0x00000003784581bdUL,
    0x00000003bd17caf2UL,
    0x000000041407e597UL,
    0x000000042f638de4UL,
    0x0000000463485d62UL,
    0x00000004b296110cUL,
    0x0000000536f2cd8fUL,
    0x000000055e1322b5UL,
    0x000000056d84cf84UL,
    0x000000056f2fca56UL,
    0x0000000586689fceUL,
    0x000000059551ef6dUL,
    0x000000059db9027bUL,
};
apriltag_family_t *tagAruco6x6_100_create()
{
   apriltag_family_t *tf = calloc(1, sizeof(apriltag_family_t));
   tf->name = strdup("tagAruco6x6_100");
   tf->h = 12;
   tf->ncodes = 100;
   tf->codes = codedata;
   tf->nbits = 36;
   tf->bit_x = calloc(36, sizeof(uint32_t));
   tf->bit_y = calloc(36, sizeof(uint32_t));
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
   tf->bit_x[5] = 2;
   tf->bit_y[5] = 2;
   tf->bit_x[6] = 3;
   tf->bit_y[6] = 2;
   tf->bit_x[7] = 4;
   tf->bit_y[7] = 2;
   tf->bit_x[8] = 3;
   tf->bit_y[8] = 3;
   tf->bit_x[9] = 6;
   tf->bit_y[9] = 1;
   tf->bit_x[10] = 6;
   tf->bit_y[10] = 2;
   tf->bit_x[11] = 6;
   tf->bit_y[11] = 3;
   tf->bit_x[12] = 6;
   tf->bit_y[12] = 4;
   tf->bit_x[13] = 6;
   tf->bit_y[13] = 5;
   tf->bit_x[14] = 5;
   tf->bit_y[14] = 2;
   tf->bit_x[15] = 5;
   tf->bit_y[15] = 3;
   tf->bit_x[16] = 5;
   tf->bit_y[16] = 4;
   tf->bit_x[17] = 4;
   tf->bit_y[17] = 3;
   tf->bit_x[18] = 6;
   tf->bit_y[18] = 6;
   tf->bit_x[19] = 5;
   tf->bit_y[19] = 6;
   tf->bit_x[20] = 4;
   tf->bit_y[20] = 6;
   tf->bit_x[21] = 3;
   tf->bit_y[21] = 6;
   tf->bit_x[22] = 2;
   tf->bit_y[22] = 6;
   tf->bit_x[23] = 5;
   tf->bit_y[23] = 5;
   tf->bit_x[24] = 4;
   tf->bit_y[24] = 5;
   tf->bit_x[25] = 3;
   tf->bit_y[25] = 5;
   tf->bit_x[26] = 4;
   tf->bit_y[26] = 4;
   tf->bit_x[27] = 1;
   tf->bit_y[27] = 6;
   tf->bit_x[28] = 1;
   tf->bit_y[28] = 5;
   tf->bit_x[29] = 1;
   tf->bit_y[29] = 4;
   tf->bit_x[30] = 1;
   tf->bit_y[30] = 3;
   tf->bit_x[31] = 1;
   tf->bit_y[31] = 2;
   tf->bit_x[32] = 2;
   tf->bit_y[32] = 5;
   tf->bit_x[33] = 2;
   tf->bit_y[33] = 4;
   tf->bit_x[34] = 2;
   tf->bit_y[34] = 3;
   tf->bit_x[35] = 3;
   tf->bit_y[35] = 4;
   tf->width_at_border = 8;
   tf->total_width = 10;
   tf->reversed_border = false;
   return tf;
}

void tagAruco6x6_100_destroy(apriltag_family_t *tf)
{
   free(tf->bit_x);
   free(tf->bit_y);
   free(tf->name);
   free(tf);
}
