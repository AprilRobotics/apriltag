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
#include "tagAruco4x4_100.h"

static uint64_t codedata[100] = {
    0x000000000000be50UL,
    0x000000000000174eUL,
    0x0000000000002db8UL,
    0x0000000000008c63UL,
    0x0000000000005a6cUL,
    0x0000000000006cafUL,
    0x000000000000997aUL,
    0x000000000000d255UL,
    0x000000000000fb4fUL,
    0x000000000000d763UL,
    0x000000000000ee86UL,
    0x0000000000000cf4UL,
    0x00000000000013f6UL,
    0x00000000000021eaUL,
    0x0000000000003294UL,
    0x0000000000003378UL,
    0x00000000000051b1UL,
    0x0000000000007100UL,
    0x000000000000726bUL,
    0x00000000000079fcUL,
    0x00000000000091ccUL,
    0x000000000000a8d8UL,
    0x000000000000d2a7UL,
    0x000000000000dc46UL,
    0x000000000000f9e3UL,
    0x0000000000009a91UL,
    0x000000000000b037UL,
    0x000000000000b621UL,
    0x00000000000024d0UL,
    0x00000000000038f9UL,
    0x00000000000052a0UL,
    0x0000000000005f54UL,
    0x00000000000099efUL,
    0x000000000000e8cdUL,
    0x000000000000007eUL,
    0x000000000000049aUL,
    0x0000000000000ab3UL,
    0x00000000000012fdUL,
    0x0000000000001677UL,
    0x0000000000001a4bUL,
    0x0000000000001f08UL,
    0x000000000000211aUL,
    0x000000000000292cUL,
    0x0000000000002a56UL,
    0x000000000000301dUL,
    0x00000000000031dfUL,
    0x00000000000036faUL,
    0x0000000000004533UL,
    0x0000000000004878UL,
    0x0000000000004ac0UL,
    0x0000000000004e24UL,
    0x0000000000005c19UL,
    0x0000000000005c83UL,
    0x0000000000005fe6UL,
    0x0000000000006082UL,
    0x00000000000060f3UL,
    0x0000000000006430UL,
    0x000000000000649dUL,
    0x0000000000006742UL,
    0x00000000000075b7UL,
    0x00000000000077edUL,
    0x0000000000007bcaUL,
    0x0000000000008014UL,
    0x0000000000008521UL,
    0x0000000000008556UL,
    0x0000000000008f59UL,
    0x0000000000009039UL,
    0x0000000000009458UL,
    0x000000000000962cUL,
    0x000000000000988eUL,
    0x0000000000009d96UL,
    0x000000000000af3bUL,
    0x000000000000b822UL,
    0x000000000000bbc9UL,
    0x000000000000bd0fUL,
    0x000000000000bddcUL,
    0x000000000000c3eaUL,
    0x000000000000c453UL,
    0x000000000000ce0bUL,
    0x000000000000cfa5UL,
    0x000000000000d20eUL,
    0x000000000000d514UL,
    0x000000000000d6f0UL,
    0x000000000000e6abUL,
    0x000000000000ecb2UL,
    0x000000000000efdeUL,
    0x000000000000f15aUL,
    0x000000000000fda9UL,
    0x0000000000003eb1UL,
    0x00000000000081beUL,
    0x0000000000007be0UL,
    0x00000000000001efUL,
    0x00000000000011c9UL,
    0x0000000000003487UL,
    0x000000000000460fUL,
    0x0000000000004735UL,
    0x0000000000005772UL,
    0x00000000000057c7UL,
    0x0000000000006437UL,
    0x00000000000068e5UL,
};
apriltag_family_t *tagAruco4x4_100_create()
{
   apriltag_family_t *tf = calloc(1, sizeof(apriltag_family_t));
   tf->name = strdup("tagAruco4x4_100");
   tf->h = 3;
   tf->ncodes = 100;
   tf->codes = codedata;
   tf->nbits = 16;
   tf->bit_x = calloc(16, sizeof(uint32_t));
   tf->bit_y = calloc(16, sizeof(uint32_t));
   tf->bit_x[0] = 1;
   tf->bit_y[0] = 1;
   tf->bit_x[1] = 2;
   tf->bit_y[1] = 1;
   tf->bit_x[2] = 3;
   tf->bit_y[2] = 1;
   tf->bit_x[3] = 2;
   tf->bit_y[3] = 2;
   tf->bit_x[4] = 4;
   tf->bit_y[4] = 1;
   tf->bit_x[5] = 4;
   tf->bit_y[5] = 2;
   tf->bit_x[6] = 4;
   tf->bit_y[6] = 3;
   tf->bit_x[7] = 3;
   tf->bit_y[7] = 2;
   tf->bit_x[8] = 4;
   tf->bit_y[8] = 4;
   tf->bit_x[9] = 3;
   tf->bit_y[9] = 4;
   tf->bit_x[10] = 2;
   tf->bit_y[10] = 4;
   tf->bit_x[11] = 3;
   tf->bit_y[11] = 3;
   tf->bit_x[12] = 1;
   tf->bit_y[12] = 4;
   tf->bit_x[13] = 1;
   tf->bit_y[13] = 3;
   tf->bit_x[14] = 1;
   tf->bit_y[14] = 2;
   tf->bit_x[15] = 2;
   tf->bit_y[15] = 3;
   tf->width_at_border = 6;
   tf->total_width = 8;
   tf->reversed_border = false;
   return tf;
}

void tagAruco4x4_100_destroy(apriltag_family_t *tf)
{
   free(tf->bit_x);
   free(tf->bit_y);
   free(tf->name);
   free(tf);
}
