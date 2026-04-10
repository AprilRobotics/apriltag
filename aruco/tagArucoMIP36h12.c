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
#include "tagArucoMIP36h12.h"

static uint64_t codedata[250] = {
    0x0000000d2a2af057UL,
    0x0000000600ce910cUL,
    0x0000000109d925dfUL,
    0x0000000fed18b3bcUL,
    0x000000087eda40adUL,
    0x0000000b14bcf6c9UL,
    0x00000006e6af268fUL,
    0x000000051193e238UL,
    0x0000000028070a6eUL,
    0x00000008a569d4daUL,
    0x0000000eb85acf5dUL,
    0x00000001bae0829eUL,
    0x0000000b2d838da9UL,
    0x000000029c0b61b6UL,
    0x00000007a33c5dccUL,
    0x000000027fcdeb19UL,
    0x00000008c8ea3fc9UL,
    0x000000048f81804bUL,
    0x0000000bd49e287cUL,
    0x0000000d8646eaadUL,
    0x000000017b9e9c8aUL,
    0x000000012426a5dcUL,
    0x0000000e2acff7faUL,
    0x00000004201cc4bbUL,
    0x000000093347a10bUL,
    0x00000002439f30b9UL,
    0x0000000c63f8f998UL,
    0x00000000047252cfUL,
    0x000000028080273dUL,
    0x000000070dadb89aUL,
    0x0000000d11aa6c8fUL,
    0x0000000f61cc0bffUL,
    0x0000000c3d9424f5UL,
    0x000000038fde83e8UL,
    0x000000025de707feUL,
    0x0000000426db0dc8UL,
    0x0000000d90712ad9UL,
    0x00000003bcd26e8bUL,
    0x0000000ad991d9dfUL,
    0x00000008b2cc69ceUL,
    0x0000000b3e0a03dbUL,
    0x0000000b70c98459UL,
    0x0000000231a4425bUL,
    0x0000000a9f8d4e2eUL,
    0x0000000aa0fd8dbcUL,
    0x00000004acaad2a8UL,
    0x00000005af8c3f29UL,
    0x0000000aab1284feUL,
    0x0000000045d42e1aUL,
    0x0000000aa5d5e24cUL,
    0x000000075ad4a5bcUL,
    0x00000008bd44978dUL,
    0x0000000e1c108a8eUL,
    0x0000000b74e419c8UL,
    0x00000002068bca9cUL,
    0x0000000f7282b5cfUL,
    0x0000000a682db0cdUL,
    0x00000001550900faUL,
    0x00000001bda9ae5dUL,
    0x00000008c34a53bfUL,
    0x00000006805541deUL,
    0x00000000c544f9cbUL,
    0x00000001c0e7e6eaUL,
    0x00000001bb5452f9UL,
    0x0000000cf4b80ebaUL,
    0x0000000d23f567adUL,
    0x00000008b399e2e3UL,
    0x000000041e92718aUL,
    0x0000000debbb12ddUL,
    0x0000000a245e3baeUL,
    0x0000000b457ae5baUL,
    0x0000000085afe92dUL,
    0x0000000f26088c6cUL,
    0x00000009d1b243e9UL,
    0x000000099c0a9cbfUL,
    0x000000082d1f68d8UL,
    0x00000008498482a8UL,
    0x00000006b1c91d9bUL,
    0x000000061f0ea608UL,
    0x00000003e2907a4dUL,
    0x00000001af514daeUL,
    0x0000000c18e689dbUL,
    0x00000008628ebe7dUL,
    0x00000003b14ae09dUL,
    0x000000035fbb6fadUL,
    0x00000000d6c28b2aUL,
    0x00000002d7488edfUL,
    0x0000000834c60214UL,
    0x000000060dd7e4afUL,
    0x0000000c84dbb1fdUL,
    0x000000012a9de16cUL,
    0x0000000c028132deUL,
    0x0000000367f8938fUL,
    0x000000046f4d318eUL,
    0x0000000ea78cc185UL,
    0x00000005604836adUL,
    0x00000002ff83b2edUL,
    0x00000002854c8810UL,
    0x0000000430327cbcUL,
    0x00000009a10b8b3cUL,
    0x00000008a590189dUL,
    0x00000008c210afefUL,
    0x00000002761c202aUL,
    0x000000026384a99eUL,
    0x00000008f57021fcUL,
    0x00000009074829c1UL,
    0x0000000810b8c01dUL,
    0x00000000f51dadb9UL,
    0x00000006a0962888UL,
    0x000000076888408eUL,
    0x00000000ce9a477cUL,
    0x0000000a5a9c62dfUL,
    0x000000048cdd86c4UL,
    0x0000000211d593ebUL,
    0x0000000adc27a988UL,
    0x0000000835024aabUL,
    0x000000040a489a1bUL,
    0x000000087e905fd9UL,
    0x0000000793da4e48UL,
    0x0000000496ece39bUL,
    0x0000000bcde17ebfUL,
    0x0000000da8069bc8UL,
    0x00000002590c1cbcUL,
    0x000000036c00aa18UL,
    0x0000000afa4f28fbUL,
    0x000000031f4275ffUL,
    0x00000004f81913e8UL,
    0x0000000078d73b9dUL,
    0x0000000c25cae0b6UL,
    0x00000005f2fb8dffUL,
    0x000000060ba20598UL,
    0x0000000af0c4da39UL,
    0x00000003a18a96ebUL,
    0x0000000d8882c77bUL,
    0x0000000121c65f8fUL,
    0x00000004dddb4bcbUL,
    0x000000073c6a09bcUL,
    0x00000009efaeebccUL,
    0x0000000eb7da27d1UL,
    0x0000000e342bc5ffUL,
    0x000000040e1f16ebUL,
    0x0000000b8356bdb9UL,
    0x0000000342a357ccUL,
    0x00000006b5ba876cUL,
    0x00000008809ed86bUL,
    0x00000008c6a2c48bUL,
    0x00000002f02e07f9UL,
    0x0000000d0c4cc198UL,
    0x000000042f148f7fUL,
    0x000000087c7b36ffUL,
    0x0000000413496ffdUL,
    0x00000007a15126baUL,
    0x0000000724e8265eUL,
    0x00000009ebc0b5aaUL,
    0x00000007c5c504e9UL,
    0x0000000c08ff4369UL,
    0x0000000913df84fcUL,
    0x000000024ad018caUL,
    0x00000009c5349afaUL,
    0x00000000393e41efUL,
    0x00000007caacde9fUL,
    0x000000062c8659ddUL,
    0x0000000d021be7c9UL,
    0x00000001b6c81009UL,
    0x000000094c8f638bUL,
    0x0000000f90582d2fUL,
    0x00000003b3e9a9e8UL,
    0x000000031dda8257UL,
    0x0000000c8e1d223cUL,
    0x0000000793845bbaUL,
    0x000000013468cf89UL,
    0x00000005a6dc7cdbUL,
    0x0000000b31be4d39UL,
    0x0000000051ebbabaUL,
    0x0000000bb8fb43aeUL,
    0x00000000aedba63bUL,
    0x0000000d242d6eebUL,
    0x00000008839d2f88UL,
    0x0000000467be23faUL,
    0x00000003bbd88fb5UL,
    0x000000069e44a979UL,
    0x0000000379223adfUL,
    0x00000002a3b6cbfdUL,
    0x00000008bbab4ff8UL,
    0x0000000e2cf9e299UL,
    0x000000070012fb9fUL,
    0x0000000a953d209fUL,
    0x0000000112158b58UL,
    0x0000000120de00e1UL,
    0x0000000e3278179cUL,
    0x0000000b395b2b69UL,
    0x0000000fdda996ceUL,
    0x000000041c8c6a6dUL,
    0x0000000bac4d497dUL,
    0x0000000689c0f3ecUL,
    0x0000000a8c1855cdUL,
    0x000000097680e6bcUL,
    0x00000001dc1c4688UL,
    0x000000018391ced6UL,
    0x0000000e161218f9UL,
    0x0000000a681b9f9bUL,
    0x0000000314f32579UL,
    0x0000000eb3aa98aeUL,
    0x000000028d6ebafdUL,
    0x00000008c1232cdeUL,
    0x0000000f2e952f9eUL,
    0x00000004659dd2edUL,
    0x000000028430f098UL,
    0x000000001dc2efd8UL,
    0x00000003d7a42a89UL,
    0x000000012f5a3798UL,
    0x000000039d7e4cdeUL,
    0x00000005115f098dUL,
    0x00000001828ea01aUL,
    0x0000000634550e09UL,
    0x000000009b3aa3d9UL,
    0x00000001ccd403bfUL,
    0x0000000ad2df818fUL,
    0x0000000f40a6c8bcUL,
    0x000000027150ff8cUL,
    0x0000000e03983b6bUL,
    0x000000048cbc0958UL,
    0x0000000a984646baUL,
    0x0000000e715302cfUL,
    0x0000000228d8572bUL,
    0x000000082d8a934eUL,
    0x00000009a7d86a38UL,
    0x0000000297dcfc6dUL,
    0x000000091c13264aUL,
    0x000000082b11c39dUL,
    0x000000001186256aUL,
    0x0000000d128e42c2UL,
    0x0000000a0a02c94fUL,
    0x00000008d5ae675fUL,
    0x0000000cadbba58aUL,
    0x0000000ccbcd61d9UL,
    0x0000000f9edb24a8UL,
    0x00000009a1a8239bUL,
    0x0000000bf4a8e9afUL,
    0x0000000e00f0edcdUL,
    0x00000004e8843ffeUL,
    0x0000000b8b8ba70fUL,
    0x00000003725249bbUL,
    0x0000000129fac6ccUL,
    0x0000000cb9cf2eadUL,
    0x0000000d84ff4f9fUL,
    0x000000043740b8dcUL,
    0x000000033ad60c5dUL,
    0x0000000713b480ceUL,
    0x00000004d79815adUL,
};
apriltag_family_t *tagArucoMIP36h12_create()
{
   apriltag_family_t *tf = calloc(1, sizeof(apriltag_family_t));
   tf->name = strdup("tagArucoMIP_36h12");
   tf->h = 12;
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

void tagArucoMIP36h12_destroy(apriltag_family_t *tf)
{
   free(tf->bit_x);
   free(tf->bit_y);
   free(tf->name);
   free(tf);
}
