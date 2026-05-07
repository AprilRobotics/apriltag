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
#include "tagAruco5x5_250.h"

static uint64_t codedata[250] = {
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
    0x00000000005530a6UL,
    0x00000000005753b3UL,
    0x00000000005a4212UL,
    0x00000000005ae3edUL,
    0x00000000005b0355UL,
    0x00000000005f0f7bUL,
    0x000000000060428eUL,
    0x00000000006096a5UL,
    0x000000000063b00bUL,
    0x00000000006f01c6UL,
    0x0000000000708980UL,
    0x000000000073ea78UL,
    0x0000000000815209UL,
    0x000000000082f7ddUL,
    0x000000000089b5abUL,
    0x00000000008b8a0dUL,
    0x000000000091976eUL,
    0x0000000000954868UL,
    0x00000000009b985eUL,
    0x00000000009ddbc8UL,
    0x0000000000a53a7eUL,
    0x0000000000a6efb3UL,
    0x0000000000a9e1f6UL,
    0x0000000000ad8dd7UL,
    0x0000000000b4c867UL,
    0x0000000000b891a4UL,
    0x0000000000ba7f81UL,
    0x0000000000bb2811UL,
    0x0000000000bc3f74UL,
    0x0000000000be72eeUL,
    0x0000000000cb3498UL,
    0x0000000000cb6f6cUL,
    0x0000000000d1a8baUL,
    0x0000000000d83a8dUL,
    0x0000000000e1fac2UL,
    0x0000000000e7103cUL,
    0x0000000000ed4975UL,
    0x0000000000f026b3UL,
    0x0000000000faf534UL,
    0x000000000101225cUL,
    0x00000000010e4832UL,
    0x00000000011179ddUL,
    0x00000000011599f4UL,
    0x000000000120b9baUL,
    0x000000000126ac68UL,
    0x000000000129332fUL,
    0x000000000130e02dUL,
    0x000000000139af1cUL,
    0x00000000013d2fb2UL,
    0x00000000013f7184UL,
    0x00000000014157c3UL,
    0x000000000143160eUL,
    0x0000000001445a16UL,
    0x00000000014d44abUL,
    0x00000000015ed0daUL,
    0x0000000001613d91UL,
    0x000000000161a061UL,
    0x000000000167d51dUL,
    0x00000000017672c8UL,
    0x000000000179c5c4UL,
    0x00000000017a9343UL,
    0x00000000017e27ecUL,
    0x00000000017fbd0aUL,
    0x000000000181819fUL,
    0x0000000001864c3dUL,
    0x000000000189103aUL,
    0x00000000018d1d98UL,
    0x000000000190144dUL,
    0x000000000190ef4aUL,
    0x0000000001977215UL,
    0x00000000019df438UL,
    0x00000000019e15b4UL,
    0x0000000001a109c4UL,
    0x0000000001a566baUL,
    0x0000000001ad5225UL,
    0x0000000001afe505UL,
    0x0000000001b050a3UL,
    0x0000000001b2ce22UL,
    0x0000000001b717cbUL,
    0x0000000001b734a0UL,
    0x0000000001bca013UL,
    0x0000000001bd68e5UL,
    0x0000000001c41d87UL,
    0x0000000001c82fcbUL,
    0x0000000001c9f151UL,
    0x0000000001ccddfbUL,
    0x0000000001cec510UL,
    0x0000000001de2263UL,
    0x0000000001deca7fUL,
    0x0000000001e40822UL,
    0x0000000001e5aca4UL,
    0x0000000001f11108UL,
    0x0000000001f19eebUL,
    0x0000000001f46bfeUL,
    0x0000000001f59172UL,
    0x0000000001f71885UL,
    0x0000000001f9b1b9UL,
    0x0000000001fbd3ddUL,
    0x0000000001fc920fUL,
    0x0000000001fdc6f0UL,
    0x0000000000c44f53UL,
    0x00000000012d3c61UL,
    0x00000000012e5703UL,
    0x0000000001c3366bUL,
    0x00000000002c17caUL,
    0x000000000056d120UL,
    0x0000000000621da8UL,
    0x00000000007ad388UL,
    0x00000000007c3cfeUL,
    0x0000000000b042d8UL,
    0x0000000000b8995bUL,
    0x0000000000d0c14fUL,
    0x0000000000f8ee30UL,
    0x0000000000fcbbddUL,
    0x00000000011f29e4UL,
    0x00000000014bce2cUL,
    0x00000000014c4df5UL,
    0x0000000001688f2fUL,
    0x000000000173d847UL,
    0x000000000182c3acUL,
    0x000000000194c673UL,
    0x0000000001a73d35UL,
    0x0000000001bc666dUL,
    0x0000000001cb3e5cUL,
    0x0000000001cfb468UL,
    0x0000000001e33634UL,
    0x00000000000017fdUL,
    0x0000000000011eb0UL,
    0x000000000001cdd4UL,
    0x00000000000246cdUL,
    0x0000000000035986UL,
    0x0000000000050e49UL,
    0x00000000000546a5UL,
    0x000000000005ecc8UL,
    0x000000000008cbecUL,
    0x00000000000a8672UL,
    0x00000000000bda00UL,
    0x00000000000be9b0UL,
    0x00000000000c8f75UL,
    0x00000000000c9c4aUL,
    0x00000000000ca068UL,
    0x00000000000e7f79UL,
    0x00000000000fa6dcUL,
    0x00000000000fc792UL,
    0x0000000000125406UL,
    0x00000000001308d4UL,
    0x000000000013497eUL,
    0x0000000000136d39UL,
    0x000000000015a41bUL,
    0x000000000015dac7UL,
};
apriltag_family_t *tagAruco5x5_250_create()
{
   apriltag_family_t *tf = calloc(1, sizeof(apriltag_family_t));
   tf->name = strdup("tagAruco5x5_250");
   tf->h = 6;
   tf->ncodes = 250;
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

void tagAruco5x5_250_destroy(apriltag_family_t *tf)
{
   free(tf->bit_x);
   free(tf->bit_y);
   free(tf->name);
   free(tf);
}
