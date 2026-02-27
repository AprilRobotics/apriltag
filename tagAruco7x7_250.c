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
#include "tagAruco7x7_250.h"

static uint64_t codedata[250] = {
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
  0x0000e575f5c45142UL,
  0x0000f4227569f686UL,
  0x0000f719b98a0dffUL,
  0x00010b20ca063fd1UL,
  0x000119fedf8a4b5dUL,
  0x000159e5d9e33a82UL,
  0x0001618b15fa5bcaUL,
  0x000176528ade090dUL,
  0x000183ae07643eeaUL,
  0x00019b733a8ac43eUL,
  0x0001a2def9dac864UL,
  0x0001a341fb77e543UL,
  0x00009769855a64f3UL,
  0x0000a6d430cf9e5aUL,
  0x000000d74d071e12UL,
  0x00000e2698ea1c92UL,
  0x00001ad454fe60c2UL,
  0x00001e584e5883a2UL,
  0x00002f6af31ca996UL,
  0x00002ff21f7e8439UL,
  0x00003abdc2cb3b40UL,
  0x00003e30bb6851f4UL,
  0x0000491333fe0abdUL,
  0x00004cb4e43f2dd9UL,
  0x00005819ee313444UL,
  0x00005d735ed1f498UL,
  0x00005e6861cb491eUL,
  0x000062808165199eUL,
  0x000063a36f92dc56UL,
  0x00006f311107d5b2UL,
  0x00009825c82bcf5fUL,
  0x00009ed46f821545UL,
  0x00009ee6e0707b62UL,
  0x0000abb3f102b70fUL,
  0x0000acabb6d6234bUL,
  0x0000acc44244b621UL,
  0x0000bb595c979f01UL,
  0x0000c03b3a4f30a6UL,
  0x0000d707bf249d90UL,
  0x0000d8e80f380d9cUL,
  0x0000e1c85a8f5145UL,
  0x0000eab421d0bc95UL,
  0x0001037915eca406UL,
  0x000107dbcea66a9bUL,
  0x000125bdcfb834baUL,
  0x00012ec1cfb51a47UL,
  0x00012fcea5f3874eUL,
  0x000137c6941cdc3cUL,
  0x0001546468139fb3UL,
  0x0001600a6efa37b5UL,
  0x00017705fad153a0UL,
  0x0001792f1bed5911UL,
  0x000179cb6a559dddUL,
  0x0001818c8ee9465cUL,
  0x000181fc31058b20UL,
  0x0001a278f56857ebUL,
  0x0001a87a326f89d1UL,
  0x0001b1e02ea1af03UL,
  0x0001b6a04057502aUL,
  0x0001c6a09f8e8768UL,
  0x0001c702d4b44ee2UL,
  0x0001cbad09bc67d7UL,
  0x0001d2e7ac516c75UL,
  0x0001d47697394091UL,
  0x0001db38fd1684a7UL,
  0x0001eeb152326ca9UL,
  0x0001fe9e1f38b941UL,
  0x00002e6b0fc08d6bUL,
  0x0000473dd887da53UL,
  0x00014c2d9857d104UL,
  0x0001655c6131c281UL,
  0x0001d678b62086deUL,
  0x0000010eaa2ff201UL,
  0x00000223b834d62aUL,
  0x000003d8090b3b6fUL,
  0x000006e6c4c53585UL,
  0x0000090348ff8646UL,
  0x00000c900ad73c8cUL,
  0x000012d7ebef400fUL,
  0x000015e33f8c7dd3UL,
  0x000019e69b5caeeeUL,
  0x00001b5d6ba0bf36UL,
  0x00002bd39a72d985UL,
  0x00002fbbd4757d6eUL,
  0x000031e623b77b86UL,
  0x000039bc31396159UL,
  0x00003fd52211b15eUL,
  0x000044f5eab18094UL,
  0x000045b81d6631fcUL,
  0x00004f3672e1a8e7UL,
  0x00005609d6ac3fc9UL,
  0x00005671dfe24d83UL,
  0x000059da148c4672UL,
  0x00005c00a58c9b15UL,
  0x00006b891c048b5eUL,
  0x00007b345a23f775UL,
  0x00007d8ad037d48dUL,
  0x000082a3485c6ad8UL,
  0x000085730bcb4840UL,
  0x0000869ea75e091aUL,
  0x000088ddb39806fbUL,
  0x0000922749b60c71UL,
  0x0000947f5cc80edaUL,
  0x0000962f93fd327eUL,
  0x0000964b6a615eefUL,
  0x00009793e05c1457UL,
  0x0000990c1ca6372eUL,
  0x000099edd150b95dUL,
  0x00009b41fad86e19UL,
  0x0000a1b24e45d38dUL,
  0x0000a22d13a8df82UL,
  0x0000a2a9bb960b94UL,
  0x0000a381adfe0f2bUL,
  0x0000aa60557d0736UL,
  0x0000ae80fae18f7dUL,
  0x0000aff2a520d464UL,
  0x0000b310c3db99b3UL,
  0x0000b7f6c7321adeUL,
  0x0000bbed2c7f5846UL,
  0x0000bc9cd95bb47eUL,
  0x0000c272384667cbUL,
  0x0000c2ca57ddaa29UL,
  0x0000cc044cf03277UL,
  0x0000d343e8018123UL,
  0x0000d9e8bba0d56fUL,
  0x0000dae1645332efUL,
  0x0000e09b403007ffUL,
  0x0000e11aa9e44355UL,
  0x0000e4748fbbe7bfUL,
  0x0000e7eb34b163e1UL,
  0x0000e89312d97051UL,
  0x0000eaff26819232UL,
  0x0000f7770069952cUL,
  0x0000f937a862b456UL,
  0x0000fba397d36e3cUL,
  0x0000fc746c5ba003UL,
  0x0000fddaf6606addUL,
  0x00010594e6e0dbd6UL,
  0x000105d295abcdfbUL,
  0x00010d94ffdb4425UL,
  0x000117b074d39f2cUL,
  0x0001185a12535639UL,
  0x00011a17e4e416a4UL,
  0x00011ad2b6049f6bUL,
  0x00011c88cc0210dfUL,
  0x00011dc2733d68a8UL,
  0x000120e1160d7319UL,
  0x00012492770d2fdeUL,
  0x000128197f157da3UL,
  0x000129be30567ed4UL,
  0x00012be5fcaa8fe2UL,
  0x00012e9cc1088abcUL,
  0x00013491d322eb45UL,
  0x000135dcd51eb7e3UL,
  0x00013b48b0b809ecUL,
  0x00013c7792fbd0efUL,
  0x00013ff89382ffa8UL,
  0x00014815b4595bfcUL,
  0x00014a3ec4f19b4bUL,
  0x00014adf08c3a405UL,
  0x00014fc654578932UL,
  0x000151408fff7feeUL,
  0x000158bf3970a9a6UL,
  0x00015998d3461810UL,
  0x00015d3a0865a730UL,
  0x00015d55ee01ce6dUL,
  0x000165645c7d7d82UL,
  0x0001677b5212baf7UL,
  0x000167f6a702b183UL,
  0x00016ad226cce5bcUL,
  0x00016b23ae7d017aUL,
  0x00016b343814600aUL,
  0x000175f26b5cb424UL,
  0x0001761617c00c20UL,
  0x00017800e2e3f43aUL,
  0x0001799d4a324a72UL,
  0x0001816f2420c5caUL,
  0x000181b99dc07f77UL,
  0x000186745e491961UL,
  0x000188913838c310UL,
  0x00018f3e2a6e028bUL,
  0x000192f5da345a6cUL,
  0x00019787d9e89f4eUL,
  0x00019945ac64db3aUL,
  0x00019d86e2b20e2fUL,
  0x0001aa18af8aef4fUL,
  0x0001aa8683ccd246UL,
  0x0001ab98e4a87150UL,
  0x0001af3adda3c31aUL,
  0x0001b062d21025e7UL,
  0x0001b25a38ac6021UL,
  0x0001b478e5738d45UL,
  0x0001c5b6c92f9505UL,
  0x0001c7d442986b79UL,
  0x0001c98a510b67b5UL,
  0x0001cc59b608fd08UL,
  0x0001cd038e4fe4bfUL,
  0x0001cf2f10c0493cUL,
  0x0001d0b6d8c778f9UL,
  0x0001d372623fd2b5UL,
  0x0001da5ee84cbec2UL
};
apriltag_family_t *tagAruco7x7_250_create()
{
  apriltag_family_t *tf = calloc(1, sizeof(apriltag_family_t));
  tf->name = strdup("tagAruco7x7_250");
  tf->h = 17;
  tf->ncodes = 250;
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

void tagAruco7x7_250_destroy(apriltag_family_t *tf)
{
  free(tf->bit_x);
  free(tf->bit_y);
  free(tf->name);
  free(tf);
}
