#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "apriltag.h"
#include "tag16h5.h"
#include "tag25h9.h"
#include "tag36h10.h"
#include "tag36h11.h"
#include "tagCircle21h7.h"
#include "tagCircle49h12.h"
#include "tagCustom48h12.h"
#include "tagStandard41h12.h"
#include "tagStandard52h13.h"

#include "../apriltag.c"

void test_family(apriltag_family_t *fam) {
    printf("Testing family %s with %d codes, %d bits\n", fam->name, fam->ncodes, fam->nbits);
    
    // Calculate theoretical max correction
    int limit = (fam->h - 1) / 2;
    if (limit > 3) limit = 3;
    printf("Family %s: h=%d, max correction tested=%d\n", fam->name, fam->h, limit);

    apriltag_detector_t *td = apriltag_detector_create();
    // Enable up to limit bits correction
    apriltag_detector_add_family_bits(td, fam, limit);
    
    // Validate initialization
    struct quick_decode *qd = (struct quick_decode*) fam->impl;
    if (!qd) {
        printf("Failed to init quick_decode for %s\n", fam->name);
        exit(1);
    }
    
    if (qd->maxhamming != limit) {
        printf("Failed to set maxhamming to %d for %s\n", limit, fam->name);
        exit(1);
    }

    int nbits = fam->nbits;
    
    for (uint32_t i = 0; i < fam->ncodes; i++) {
        uint64_t code = fam->codes[i];
        
        // Test 0 errors
        struct quick_decode_result res;
        quick_decode_codeword(fam, code, &res);
        if (res.id != i || res.hamming != 0) {
            printf("Failed 0 errors: code %u, got id %d hamming %d\n", i, res.id, res.hamming);
            exit(1);
        }
        
        if (limit < 1) continue;

        // Test 1 bit error
        for (int b1 = 0; b1 < nbits; b1++) {
            uint64_t c1 = code ^ (1ULL << b1);
            quick_decode_codeword(fam, c1, &res);
             if (res.id != i || res.hamming != 1) {
                printf("Failed 1 error: code %u bit %d, got id %d hamming %d\n", i, b1, res.id, res.hamming);
                exit(1);
            }
            
            if (limit < 2) continue;

            // Test 2 bit errors
            for (int b2 = b1 + 1; b2 < nbits; b2++) {
                uint64_t c2 = c1 ^ (1ULL << b2);
                quick_decode_codeword(fam, c2, &res);
                if (res.id != i || res.hamming != 2) {
                    printf("Failed 2 errors: code %u bits %d,%d, got id %d hamming %d\n", i, b1, b2, res.id, res.hamming);
                    exit(1);
                }

                if (limit < 3) continue;

                // Test 3 bit errors
                for (int b3 = b2 + 1; b3 < nbits; b3++) {
                    uint64_t c3 = c2 ^ (1ULL << b3);
                    quick_decode_codeword(fam, c3, &res);
                    if (res.id != i || res.hamming != 3) {
                        printf("Failed 3 errors: code %u bits %d,%d,%d, got id %d hamming %d\n", i, b1, b2, b3, res.id, res.hamming);
                        exit(1);
                    }
                }
            }
        }
    }
    
    apriltag_detector_destroy(td);
    printf("Family %s passed.\n", fam->name);
}

int main() {
    apriltag_family_t *fams[] = {
        tag16h5_create(),
        tag25h9_create(),
        tag36h10_create(),
        tag36h11_create(),
        tagCircle21h7_create(),
        tagCircle49h12_create(),
        tagCustom48h12_create(),
        tagStandard41h12_create(),
        tagStandard52h13_create(),
        NULL
    };

    for (int i = 0; fams[i]; i++) {
        test_family(fams[i]);
    }

    tag16h5_destroy(fams[0]);
    tag25h9_destroy(fams[1]);
    tag36h10_destroy(fams[2]);
    tag36h11_destroy(fams[3]);
    tagCircle21h7_destroy(fams[4]);
    tagCircle49h12_destroy(fams[5]);
    tagCustom48h12_destroy(fams[6]);
    tagStandard41h12_destroy(fams[7]);
    tagStandard52h13_destroy(fams[8]);

    printf("All quick_decode tests passed!\n");
    return 0;
}
