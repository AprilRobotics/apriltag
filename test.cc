/*
 * Copyright 2020 Johannes Schauer <josch@mister-muffin.de>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 */

#include <iostream>
#include <bitset>
#include <assert.h>

#include "opencv2/opencv.hpp"

extern "C" {
#include "apriltag.h"
#include "tag36h11.h"
}

using namespace std;
using namespace cv;


int main(int argc, char *argv[])
{
	bitset<64> tag0(string(
				"00000000"
				"01101010"
				"00111010"
				"00110000"
				"01010000"
				"00101100"
				"00001000"
				"00000000"
				));
	Mat tag0_img(8, 8, CV_8U, Scalar(0,0,0));
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			tag0_img.at<uchar>(i, j, 0) = 255*tag0[(7-i)*8+(7-j)];
		}
	}
	Mat enlarged(128, 128, CV_8U, Scalar(0,0,0));
	resize(tag0_img, enlarged, enlarged.size(), 0, 0, INTER_NEAREST);
	Mat frame(256, 256, CV_8U, Scalar(255,255,255));
	enlarged.copyTo(frame(Rect(Point(64,64),enlarged.size())));
	Point2f src_pts[8] = { Point2f(0,0),
		Point2f(256,0),
		Point2f(0,256),
		Point2f(256,256)};
	Point2f dst_pts[8] = { Point2f(20,0),
		Point2f(226,30),
		Point2f(20,256),
		Point2f(226,226)};
	Mat transmat = getPerspectiveTransform(src_pts, dst_pts);
	Mat frame_persp(frame.size(), CV_8U, Scalar(255,255,255));
	warpPerspective(frame, frame_persp, transmat, frame_persp.size());

	// Initialize tag detector with options
	apriltag_family_t *tf = NULL;
	tf = tag36h11_create();

	apriltag_detector_t *td = apriltag_detector_create();
	apriltag_detector_add_family(td, tf);
	td->quad_decimate = 2.0;
	td->quad_sigma = 0.0;
	td->nthreads = 1;
	td->debug = 1;
	td->refine_edges = 1;

	// designated initializers only get introduced in c++20
	image_u8_t im = {
		frame_persp.cols,
		frame_persp.rows,
		frame_persp.cols,
		frame_persp.data
	};

	zarray_t *detections = apriltag_detector_detect(td, &im);

	assert(zarray_size(detections)==1);

	apriltag_detection_t *det;
	zarray_get(detections, 0, &det);

	float eps = 0.001;
	assert(0==det->id);
	assert(fabs(136.641-det->c[0]) <= eps);
	assert(fabs(127.998-det->c[1]) <= eps);
	assert(fabs(82.4142-det->p[0][0]) <= eps);
	assert(fabs(187.541-det->p[0][1]) <= eps);
	assert(fabs(184.206-det->p[1][0]) <= eps);
	assert(fabs(180.157-det->p[1][1]) <= eps);
	assert(fabs(184.036-det->p[2][0]) <= eps);
	assert(fabs(75.9571-det->p[2][1]) <= eps);
	assert(fabs(82.4142-det->p[3][0]) <= eps);
	assert(fabs(68.5336-det->p[3][1]) <= eps);

	zarray_destroy(detections);

	apriltag_detector_destroy(td);
	tag36h11_destroy(tf);

	return 0;
}
