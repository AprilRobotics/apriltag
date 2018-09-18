INSTALL
=======

The default installation will place headers in /usr/local/include and
shared library in /usr/local/lib. It also installs a pkg-config script
into /usr/local/lib/pkgconfig.

    $ make
    $ sudo make install

To install to a different directory than /usr/local:

    $ PREFIX=/some/path sudo make install


USAGE
=====

A basic AprilTag application can be seen in example/apriltag_demo.c.


Initialization: instantiate a detector and at least one tag family.

    apriltag_detector_t *td = apriltag_detector_create();
    apriltag_family_t *tf = tag36h11_create();
    apriltag_detector_add_family(td, tf);

Some tag detector parameters can be set at this time.
The default parameters are the recommended starting point.

    td->quad_decimate = 1.0;
    td->quad_sigma = 0.0;
    td->refine_edges = 1;
    td->refine_decode = 0;
    td->refine_pose = 0;

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


OPENCV INTEGRATION
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
