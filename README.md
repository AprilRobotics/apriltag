AprilTag 3
==========
AprilTag is a visual fiducial system popular in robotics research. This repository contains the most recent version of AprilTag, AprilTag 3, which includes a faster (>2x) detector, improved detection rate on small tags, flexible tag layouts, and pose estimation. AprilTag consists of a small C library with minimal dependencies. Officially only linux operating systems are supported.

You can find tag images for the pre-generated layouts [here](https://github.com/AprilRobotics/apriltag-imgs). We recommend using the tagStandard41h12 layout.

[![Build Status](https://travis-ci.org/AprilRobotics/apriltag.svg?branch=master)](https://travis-ci.org/AprilRobotics/apriltag)

Install
=======

The default installation will place headers in /usr/local/include and
shared library in /usr/local/lib. It also installs a pkg-config script
into /usr/local/lib/pkgconfig and will install a python wrapper if python3 is installed. Be aware that there are some larger tag families which may take a long time to build. If you do not want to use these tag families then you can speed up the installation by deleting the files tagCircle49h12.c, tagCircle49h12.h, tagCustom48h12.c, tagCustom48h12.h, tagStandard52h13.c, and tagStandard52h13.h before installing.

If you have CMake installed or it is not difficult to install, then do:

    $ cmake .
    $ sudo make install
    
Otherwise, we have a handwritten makefile you can use (be warned it will do slightly different things):

    $ make
    $ sudo make install
    
To install to a different directory than /usr/local:

    $ PREFIX=/some/path sudo make install


Usage
=====

We recommend using the tagStandard41h12 family for all new application.

A basic AprilTag application can be seen in example/apriltag_demo.c.


Initialization: instantiate a detector and at least one tag family.

    apriltag_detector_t *td = apriltag_detector_create();
    apriltag_family_t *tf = tag36h11_create();
    apriltag_detector_add_family(td, tf);

Some tag detector parameters can be set at this time.
The default parameters are the recommended starting point.

    td->quad_decimate = 2.0;
    td->quad_sigma = 0.0;
    td->refine_edges = 1;
    td->decode_sharpening = 0.25;

Increase the image decimation if faster processing is required; the
trade-off is a slight decrease in detection range. A factor of 1.0
means the full-size input image is used.

Some Gaussian blur (quad_sigma) may help with noisy input images.


Detection: a single one-line call will process an input image
and return a list of detections.

    zarray_t *detections = apriltag_detector_detect(td, im);

    for (int i = 0; i < zarray_size(detections); i++) {
        apriltag_detection_t *det;
        zarray_get(detections, i, &det);

        // Do something with det here
    }

    apriltag_detections_destroy(detections);

zarray is a container class which is included with apriltag.
To process through the list of detections, use zarray_get,
as illustrated above.

The caller is responsible for freeing detections by calling
apriltag_detections_destroy().


Cleanup: free the detector and tag family when done.

    apriltag_detector_destroy(td);
    tag36h11_destroy(tf);

Python
======
The python wrapper will be installed if the system has python3 installed. Usage is as follows:

    import cv2
    import numpy as np
    from apriltag import apriltag

    imagepath = '/tmp/tst.jpg'
    image     = cv2.imread(imagepath, cv2.IMREAD_GRAYSCALE)
    detector = apriltag("tag36h11")

    detections = detector.detect(image)


Support
=======
Please create an issue on this github for any questions instead of sending a private message. This allows other people with the same question to find your answer.

Flexible Layouts
================
AprilTag 3 supports a wide variety of possible tag layouts in addition to the classic layout supported in AprilTag 2. The tag's data bits can now go outside of the tag border, and it is also possible to define layouts with "holes" inside of the tag border where there are no data bits. In this repo we have included:

* Two families of the new standard layout. This layout adds a layer of data bits around the outside of the tag border, increasing data density, and the number of possible tags, at the cost of a slight decrease in detection distance.
* Two families of circular tags.
* One family which has a hole in the middle. This could be used for example for drone applications by placing different sized tags inside of each other to allow detection over a wide range of distances.

You can generate your own tag families using our other repo, [AprilTag-Generation](https://github.com/AprilRobotics/apriltag-generation).

Pose Estimation
===============
We have added methods to estimate the 3d pose of the AprilTag given camera parameters and the size of the tag. Sample code is as follows:

    // First create an apriltag_detection_info_t struct using your known parameters.
    apriltag_detection_info_t info;
    info.det = det;
    info.tagsize = tagsize;
    info.fx = fx;
    info.fy = fy;
    info.cx = cx;
    info.cy = cy;

    // Then call estimate_tag_pose.
    apriltag_pose_t pose;
    double err = estimate_tag_pose(&info, &pose);
    
    // Do something with pose.
    ...
    
You can also call <code>estimate_tag_pose_orthogonal_iteration</code> which allows the user to specify the number of iterations used and also returns both possible solutions for the tag pose along with their errors.


Upgrading from AprilTag 2
=========================
For most use-cases this should be a drop in replacement.

* The options refine_decode, refine_pose, and black_border have been removed.
* If you have generated your own families, you will need to regenerate the c code for those families. The java code however does not need to be regenerated so this should be quick and easy.




OpenCV Integration
==================

Note that this library has no external dependencies. Most applications
will require, at minimum, a method for acquiring images.

See example/opencv_demo.cc for an example of using AprilTag in C++ with OpenCV.
This example application can be built by executing the following:

    $ cd examples
    $ make opencv_demo

Image data in a cv::Mat object can be passed to AprilTag without creating
a deep copy. Simply create an image_u8_t header for the cv::Mat data buffer:

    cv::Mat img;

    image_u8_t img_header = { .width = img.cols,
        .height = img.rows,
        .stride = img.cols,
        .buf = img.data
    };
    
Wrappers
========
Third-party wrappers of the apriltag code for other languages.
 
[Python](https://github.com/duckietown/apriltags3-py)

[Matlab](https://github.com/alddiaz/MATLAB_AprilTag3)
