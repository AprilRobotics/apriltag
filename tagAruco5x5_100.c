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
#include "tagAruco5x5_100.h"

static uint64_t codedata[100] = {
    0x000000000152c6e3UL,
    0x0000000000158da8UL,
    0x0000000001b977e8UL,
    0x00000000010bddc5UL,
    0x0000000001bb840fUL,
    0x0000000001d4b600UL,
    0x0000000000cfc7b5UL,
    0x0000000000e99b01UL,
    0x000000000110c85eUL,
    0x000000000125f4a7UL,
    0x000000000137383aUL,
    0x0000000001aa21b1UL,
    0x0000000001f83066UL,
    0x00000000005c8d1fUL,
    0x0000000001f7a29bUL,
    0x000000000046df76UL,
    0x00000000009f4366UL,
    0x0000000000be9f2aUL,
    0x0000000000fd2594UL,
    0x000000000119a385UL,
    0x000000000132f579UL,
    0x0000000001470112UL,
    0x000000000169f088UL,
    0x0000000000ac0fe9UL,
    0x0000000001969819UL,
    0x0000000001b2e945UL,
    0x0000000001cb7ab0UL,
    0x0000000000280d30UL,
    0x00000000002fc969UL,
    0x0000000000309a62UL,
    0x000000000039cf27UL,
    0x00000000003eb940UL,
    0x0000000000420f91UL,
    0x00000000004a23aaUL,
    0x0000000000588af4UL,
    0x00000000006dabbeUL,
    0x000000000086e6a8UL,
    0x00000000008a1015UL,
    0x00000000008f364eUL,
    0x000000000099a943UL,
    0x0000000000b0f40aUL,
    0x0000000000b16ee6UL,
    0x0000000000c4bddcUL,
    0x0000000000c873c4UL,
    0x0000000000cdb07bUL,
    0x0000000000e0cb7aUL,
    0x0000000000e27ccfUL,
    0x0000000000e46c18UL,
    0x0000000000f6d405UL,
    0x0000000000ff1de7UL,
    0x00000000010ce599UL,
    0x00000000010e9379UL,
    0x00000000011c3a0bUL,
    0x00000000011f1501UL,
    0x00000000013a6b3fUL,
    0x00000000014636bcUL,
    0x000000000147c1ecUL,
    0x0000000001493fe6UL,
    0x0000000001589c29UL,
    0x00000000015fa835UL,
    0x00000000016ef1f5UL,
    0x000000000172a654UL,
    0x0000000001762cafUL,
    0x0000000001803ca8UL,
    0x00000000018557f4UL,
    0x0000000001922ab6UL,
    0x000000000193d68cUL,
    0x0000000001afb294UL,
    0x0000000001afb94fUL,
    0x0000000001e3c859UL,
    0x0000000001f9c216UL,
    0x0000000000f54a13UL,
    0x0000000001a47961UL,
    0x000000000007b538UL,
    0x00000000002a126bUL,
    0x000000000086309eUL,
    0x0000000000a10668UL,
    0x0000000001424346UL,
    0x0000000001904707UL,
    0x00000000019e8615UL,
    0x0000000001ee88bdUL,
    0x000000000000d4eaUL,
    0x000000000007e18bUL,
    0x000000000009c759UL,
    0x00000000000b9f34UL,
    0x00000000000c52a2UL,
    0x00000000000e9cb1UL,
    0x000000000010393cUL,
    0x00000000001835b7UL,
    0x00000000001a762aUL,
    0x00000000001d656dUL,
    0x000000000026b35aUL,
    0x00000000002cb827UL,
    0x0000000000311d2bUL,
    0x0000000000335a4eUL,
    0x00000000003b368dUL,
    0x00000000003bc914UL,
    0x00000000003fb5dfUL,
    0x00000000004da1a1UL,
    0x000000000053b7e0UL,
};
apriltag_family_t *tagAruco5x5_100_create()
{
   apriltag_family_t *tf = calloc(1, sizeof(apriltag_family_t));
   tf->name = strdup("tagAruco5x5_100");
   tf->h = 7;
   tf->ncodes = 100;
   tf->codes = codedata;
   tf->nbits = 25;
   tf->bit_x = calloc(25, sizeof(uint32_t));
   tf->bit_y = calloc(25, sizeof(uint32_t));
   tf->bit_x[0] = 1;
   tf->bit_y[0] = 1;
   tf->bit_x[1] = 2;
   tf->bit_y[1] = 1;
   tf->bit_x[2] = 3;
   tf->bit_y[2] = 1;
   tf->bit_x[3] = 4;
   tf->bit_y[3] = 1;
   tf->bit_x[4] = 2;
   tf->bit_y[4] = 2;
   tf->bit_x[5] = 3;
   tf->bit_y[5] = 2;
   tf->bit_x[6] = 5;
   tf->bit_y[6] = 1;
   tf->bit_x[7] = 5;
   tf->bit_y[7] = 2;
   tf->bit_x[8] = 5;
   tf->bit_y[8] = 3;
   tf->bit_x[9] = 5;
   tf->bit_y[9] = 4;
   tf->bit_x[10] = 4;
   tf->bit_y[10] = 2;
   tf->bit_x[11] = 4;
   tf->bit_y[11] = 3;
   tf->bit_x[12] = 5;
   tf->bit_y[12] = 5;
   tf->bit_x[13] = 4;
   tf->bit_y[13] = 5;
   tf->bit_x[14] = 3;
   tf->bit_y[14] = 5;
   tf->bit_x[15] = 2;
   tf->bit_y[15] = 5;
   tf->bit_x[16] = 4;
   tf->bit_y[16] = 4;
   tf->bit_x[17] = 3;
   tf->bit_y[17] = 4;
   tf->bit_x[18] = 1;
   tf->bit_y[18] = 5;
   tf->bit_x[19] = 1;
   tf->bit_y[19] = 4;
   tf->bit_x[20] = 1;
   tf->bit_y[20] = 3;
   tf->bit_x[21] = 1;
   tf->bit_y[21] = 2;
   tf->bit_x[22] = 2;
   tf->bit_y[22] = 4;
   tf->bit_x[23] = 2;
   tf->bit_y[23] = 3;
   tf->bit_x[24] = 3;
   tf->bit_y[24] = 3;
   tf->width_at_border = 7;
   tf->total_width = 9;
   tf->reversed_border = false;
   return tf;
}

void tagAruco5x5_100_destroy(apriltag_family_t *tf)
{
   free(tf->bit_x);
   free(tf->bit_y);
   free(tf->name);
   free(tf);
}
