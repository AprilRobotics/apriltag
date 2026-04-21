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
#include "tagAruco6x6_250.h"

static uint64_t codedata[250] = {
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
    0x00000005cceff55dUL,
    0x00000005d229255bUL,
    0x000000064c9ddd62UL,
    0x000000067eccda0cUL,
    0x00000006a201db7aUL,
    0x00000006f1dd0046UL,
    0x00000006fb415109UL,
    0x00000007177d99e8UL,
    0x00000007388f55faUL,
    0x000000076734a21dUL,
    0x00000007848ba632UL,
    0x000000079167847cUL,
    0x00000007ab338c0dUL,
    0x00000007f094be0aUL,
    0x000000081de59cbfUL,
    0x000000083b6abd5eUL,
    0x000000085c88a26bUL,
    0x000000098294ab90UL,
    0x000000099727b0c8UL,
    0x00000009f6547e8cUL,
    0x0000000a0a0865beUL,
    0x0000000a9620c395UL,
    0x0000000ac4c618c0UL,
    0x0000000b02eddb87UL,
    0x0000000b1d50b035UL,
    0x0000000b4ea4b14eUL,
    0x0000000badbb67b8UL,
    0x0000000bd0e3def5UL,
    0x0000000c1ffa1654UL,
    0x0000000c79a75f83UL,
    0x0000000cd31efa5bUL,
    0x0000000ce1a88d1cUL,
    0x0000000d00ce33a9UL,
    0x0000000d1068a8efUL,
    0x0000000d7264a958UL,
    0x0000000e3caf9955UL,
    0x0000000e5a4d46d5UL,
    0x0000000e8818ae83UL,
    0x0000000e8d17b80aUL,
    0x0000000eba757c10UL,
    0x0000000f0eac5af8UL,
    0x0000000f48566d17UL,
    0x0000000f9c1e79f5UL,
    0x0000000ae1e743d7UL,
    0x0000000c330f5948UL,
    0x0000000179cb95f7UL,
    0x00000004a597a65dUL,
    0x0000000535280f54UL,
    0x0000000692780819UL,
    0x00000007203064b5UL,
    0x00000008530c1caaUL,
    0x0000000cd5dbe8baUL,
    0x0000000d2fe483f0UL,
    0x0000000ebe8e2584UL,
    0x0000000f45b78644UL,
    0x0000000fcad7bbeaUL,
    0x0000000001339f4eUL,
    0x00000000018112a1UL,
    0x00000000073a4daaUL,
    0x000000001f262424UL,
    0x000000001f32e2f8UL,
    0x00000000283b13a4UL,
    0x000000003604b4cdUL,
    0x0000000046517041UL,
    0x0000000050138044UL,
    0x00000000625617afUL,
    0x00000000838b678cUL,
    0x0000000098fd8fa4UL,
    0x00000000b431447dUL,
    0x00000000d12b7ef6UL,
    0x00000000f6d350f7UL,
    0x00000000fd981f31UL,
    0x0000000104b87a8fUL,
    0x000000010c46f942UL,
    0x000000014447a8acUL,
    0x00000001539b8e36UL,
    0x00000001581dbd87UL,
    0x00000001a6b46561UL,
    0x00000001b6cd8f2aUL,
    0x00000001cd634677UL,
    0x00000001d96ba10aUL,
    0x00000001db14928eUL,
    0x00000001e5308efcUL,
    0x00000001e9487698UL,
    0x00000001e9f1f172UL,
    0x0000000220795e56UL,
    0x000000022a37c936UL,
    0x0000000240e00eb6UL,
    0x000000025ed0c569UL,
    0x0000000264bfb6d3UL,
    0x00000002762a830bUL,
    0x0000000299e1f28eUL,
    0x000000029d481345UL,
    0x00000002a3643ef5UL,
    0x00000002d99309caUL,
    0x00000002ec20204bUL,
    0x00000002fa2599f0UL,
    0x000000032518f73eUL,
    0x000000032dcf9e4eUL,
    0x0000000337dc00e3UL,
    0x0000000367de375cUL,
    0x00000003887aab11UL,
    0x000000038e94425dUL,
    0x00000003a78911a8UL,
    0x00000003d2d26b25UL,
    0x00000003d37b558fUL,
    0x00000003d4362f7fUL,
    0x00000003dd34c828UL,
    0x00000003e8afb268UL,
    0x000000040d49e611UL,
    0x000000042c18f155UL,
    0x0000000431b5db39UL,
    0x00000004349fefe4UL,
    0x0000000439565b06UL,
    0x000000043e51d2dcUL,
    0x0000000443cf493cUL,
    0x00000004497a7ac8UL,
    0x000000045e975bd0UL,
    0x000000048f9044feUL,
    0x000000049d5009b0UL,
    0x000000049e5fc173UL,
    0x00000004a04f08f6UL,
    0x00000004c2166f86UL,
    0x00000004d59e052fUL,
    0x00000004e9a614daUL,
    0x00000004f299d88aUL,
    0x00000005046ccfb9UL,
    0x000000055bf0ffbeUL,
    0x0000000576fd77c2UL,
    0x00000005775fc44bUL,
    0x000000058add7861UL,
    0x00000005ae7af43eUL,
    0x00000005c792b9d5UL,
    0x00000005cfd8e6a7UL,
    0x00000005d6b50e99UL,
    0x00000005f46c430dUL,
    0x000000060aff8445UL,
    0x00000006164a1dadUL,
    0x0000000671d1a7b5UL,
    0x000000067ac2f3e5UL,
    0x000000068e6975f0UL,
    0x0000000694e62f90UL,
    0x00000006a094d4f9UL,
    0x00000006aa4eb15aUL,
    0x00000006cb9a0090UL,
    0x00000006d1a94531UL,
    0x00000006e7d2691aUL,
    0x00000006ed3ac6c7UL,
    0x00000006fd7ba039UL,
    0x00000007026405e7UL,
};
apriltag_family_t *tagAruco6x6_250_create()
{
   apriltag_family_t *tf = calloc(1, sizeof(apriltag_family_t));
   tf->name = strdup("tagAruco6x6_250");
   tf->h = 11;
   tf->ncodes = 250;
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

void tagAruco6x6_250_destroy(apriltag_family_t *tf)
{
   free(tf->bit_x);
   free(tf->bit_y);
   free(tf->name);
   free(tf);
}
