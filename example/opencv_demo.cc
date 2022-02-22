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

#include <iostream>
#include <cstdlib>

#include "opencv2/opencv.hpp"
#include "opencv2/core/utility.hpp"

extern "C" {
#include "apriltag.h"
#include "tag36h11.h"
#include "tag25h9.h"
#include "tag16h5.h"
#include "tagCircle21h7.h"
#include "tagCircle49h12.h"
#include "tagCustom48h12.h"
#include "tagStandard41h12.h"
#include "tagStandard52h13.h"

#include "common/image_u8.h"
#include "example/getopt.h"
#include "common/image_u8x3.h"
}

using namespace std;
using namespace cv;


//
// Capture routines based on diagnostic tracing routines
//
#if defined (AT_DIAG_ENABLE_TRACING)

static image_u8_t *s_capture_preprocess = NULL;
static image_u8_t *s_capture_threshold = NULL;
static image_u8x3_t *s_capture_quads = NULL;

static void capture_init() {
}

static void try_copy_image(image_u8_t **dest, const image_u8_t *src)
{
    if (!*dest) {
        *dest = image_u8_copy(src);
        return;
    }

    if (src->width != s_capture_preprocess->width || src->height != s_capture_preprocess->height) {
        image_u8_destroy(s_capture_preprocess);
        *dest = image_u8_copy(src);
        return;
    }

    AT_ASSERT(src->stride == s_capture_preprocess->stride);

    memcpy((*dest)->buf, src->buf, src->height * src->stride * sizeof(uint8_t));
}

extern "C" void capture_preprocess(const image_u8_t *src)
{
    try_copy_image(&s_capture_preprocess, src);
}

extern "C" void capture_quads_threshold(const image_u8_t *src)
{
    try_copy_image(&s_capture_threshold, src);
}

extern "C" void capture_quads_final(const image_u8_t *im_orig, const struct zarray *quads)
{
    srandom(0);

    for (int i = 0; i < zarray_size(quads); i++) {
        struct quad *q;
        zarray_get_volatile(quads, i, &q);

        float rgb[3];
        int bias = 100;

        for (int j = 0; j < 3; j++) {
            rgb[j] = bias + (random() % (255-bias));
        }

//        fprintf(f, "%f %f %f setrgbcolor\n", rgb[0]/255.0f, rgb[1]/255.0f, rgb[2]/255.0f);
//        fprintf(f, "%f %f moveto %f %f lineto %f %f lineto %f %f lineto %f %f lineto stroke\n",
//                q->p[0][0], q->p[0][1],
//                q->p[1][0], q->p[1][1],
//                q->p[2][0], q->p[2][1],
//                q->p[3][0], q->p[3][1],
//                q->p[0][0], q->p[0][1]);
    }
}


#endif


int main(int argc, char *argv[])
{
    getopt_t *getopt = getopt_create();

    getopt_add_bool(getopt, 'h', "help", 0, "Show this help");
    getopt_add_bool(getopt, 'q', "quiet", 0, "Reduce output");
    getopt_add_string(getopt, 'f', "family", "tag36h11", "Tag family to use");
    getopt_add_int(getopt, 't', "threads", "1", "Use this many CPU threads");
    getopt_add_double(getopt, 'x', "decimate", "2.0", "Decimate input image by this factor");
    getopt_add_double(getopt, 'b', "blur", "0.0", "Apply low-pass blur to input");
    getopt_add_bool(getopt, '0', "refine-edges", 1, "Spend more time trying to align edges of tags");

    if (!getopt_parse(getopt, argc, argv, 1) ||
            getopt_get_bool(getopt, "help")) {
        printf("Usage: %s [options]\n", argv[0]);
        getopt_do_usage(getopt);
        exit(0);
    }

    cout << "Enabling video capture" << endl;
    
    TickMeter meter;
    meter.start();

    // Initialize camera
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "Couldn't open video capture device" << endl;
        return -1;
    }
    meter.stop();

    cout << "VideoCapture initialized in " 
        << std::fixed << std::setprecision(3) << meter.getTimeSec() << " seconds" << endl;
    meter.reset();
    meter.start();

    // Initialize tag detector with options
    apriltag_family_t *tf = NULL;
    const char *famname = getopt_get_string(getopt, "family");
    if (!strcmp(famname, "tag36h11")) {
        tf = tag36h11_create();
    } else if (!strcmp(famname, "tag25h9")) {
        tf = tag25h9_create();
    } else if (!strcmp(famname, "tag16h5")) {
        tf = tag16h5_create();
    } else if (!strcmp(famname, "tagCircle21h7")) {
        tf = tagCircle21h7_create();
    } else if (!strcmp(famname, "tagCircle49h12")) {
        tf = tagCircle49h12_create();
    } else if (!strcmp(famname, "tagStandard41h12")) {
        tf = tagStandard41h12_create();
    } else if (!strcmp(famname, "tagStandard52h13")) {
        tf = tagStandard52h13_create();
    } else if (!strcmp(famname, "tagCustom48h12")) {
        tf = tagCustom48h12_create();
    } else {
        printf("Unrecognized tag family name. Use e.g. \"tag36h11\".\n");
        exit(-1);
    }

    meter.stop();
    cout << "Tag family " << famname << " initialized in " 
        << std::fixed << std::setprecision(3) << meter.getTimeSec() << " seconds" << endl;
    meter.reset();
    meter.start();

    apriltag_detector_t *td = apriltag_detector_create();
    apriltag_detector_add_family(td, tf);
    td->quad_decimate = getopt_get_double(getopt, "decimate");
    td->quad_sigma = getopt_get_double(getopt, "blur");
    td->nthreads = getopt_get_int(getopt, "threads");
    td->refine_edges = getopt_get_bool(getopt, "refine-edges");


#if defined (AT_DIAG_ENABLE_TRACING)
    td->diag.preprocess = capture_preprocess;
    td->diag.quads_threshold = capture_quads_threshold;
    td->diag.quads_final = capture_quads_final;
#endif

    float frame_counter = 0.0f;
    meter.stop();
    cout << "Detector " << famname << " initialized in " 
        << std::fixed << std::setprecision(3) << meter.getTimeSec() << " seconds" << endl;
    cout << "  " << cap.get(CAP_PROP_FRAME_WIDTH ) << "x" <<
                    cap.get(CAP_PROP_FRAME_HEIGHT ) << " @" <<
                    cap.get(CAP_PROP_FPS) << "FPS" << endl;
    meter.reset();

#if defined(AT_DIAG_ENABLE_TRACING)
    Mat compose;
#endif

    Mat frame, gray;

    while (true) {
        meter.start();
        cap >> frame;

        cvtColor(frame, gray, COLOR_BGR2GRAY);

        // Make an image_u8_t header for the Mat data
        image_u8_t im = { .width = gray.cols,
            .height = gray.rows,
            .stride = gray.cols,
            .buf = gray.data
        };

        zarray_t *detections = apriltag_detector_detect(td, &im);

        // Draw detection outlines
        for (int i = 0; i < zarray_size(detections); i++) {
            apriltag_detection_t *det;
            zarray_get(detections, i, &det);
            line(frame, Point(det->p[0][0], det->p[0][1]),
                     Point(det->p[1][0], det->p[1][1]),
                     Scalar(0, 0xff, 0), 2);
            line(frame, Point(det->p[0][0], det->p[0][1]),
                     Point(det->p[3][0], det->p[3][1]),
                     Scalar(0, 0, 0xff), 2);
            line(frame, Point(det->p[1][0], det->p[1][1]),
                     Point(det->p[2][0], det->p[2][1]),
                     Scalar(0xff, 0, 0), 2);
            line(frame, Point(det->p[2][0], det->p[2][1]),
                     Point(det->p[3][0], det->p[3][1]),
                     Scalar(0xff, 0, 0), 2);

            stringstream ss;
            ss << det->id;
            String text = ss.str();
            int fontface = FONT_HERSHEY_SCRIPT_SIMPLEX;
            double fontscale = 1.0;
            int baseline;
            Size textsize = getTextSize(text, fontface, fontscale, 2,
                                            &baseline);
            putText(frame, text, Point(det->c[0]-textsize.width/2,
                                       det->c[1]+textsize.height/2),
                    fontface, fontscale, Scalar(0xff, 0x99, 0), 2);
        }
        apriltag_detections_destroy(detections);



#if defined(AT_DIAG_ENABLE_TRACING)
//        image_u8_t *right_hand = s_capture_preprocess;
//        Mat rightHand();
//        Mat *concat[2] = {&frame, &rightHand};
//        hconcat(concat, 2, compose);

        imshow("Tag Tracing", compose);
#else
        imshow("Tag Detections", frame);
#endif

        // add a delay if desired
        if (waitKey(0) >= 0) {
            break;
        }

        frame_counter++;
        if (frame_counter > 30) {
            cout << "FPS "
                << std::fixed << std::setprecision(3) << (meter.getCounter() / meter.getTimeSec()) << endl;
            frame_counter = 0.0f;
        }
        meter.stop();
    }

    apriltag_detector_destroy(td);

    if (!strcmp(famname, "tag36h11")) {
        tag36h11_destroy(tf);
    } else if (!strcmp(famname, "tag25h9")) {
        tag25h9_destroy(tf);
    } else if (!strcmp(famname, "tag16h5")) {
        tag16h5_destroy(tf);
    } else if (!strcmp(famname, "tagCircle21h7")) {
        tagCircle21h7_destroy(tf);
    } else if (!strcmp(famname, "tagCircle49h12")) {
        tagCircle49h12_destroy(tf);
    } else if (!strcmp(famname, "tagStandard41h12")) {
        tagStandard41h12_destroy(tf);
    } else if (!strcmp(famname, "tagStandard52h13")) {
        tagStandard52h13_destroy(tf);
    } else if (!strcmp(famname, "tagCustom48h12")) {
        tagCustom48h12_destroy(tf);
    }


    getopt_destroy(getopt);

    return 0;
}
