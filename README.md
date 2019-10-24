AprilTag 3
==========
AprilTag is a visual fiducial system popular in robotics research. This repository contains the most recent version of AprilTag, AprilTag 3, which includes a faster (>2x) detector, improved detection rate on small tags, flexible tag layouts, and pose estimation. AprilTag consists of a small C library with minimal dependencies.

You can find tag images for the pre-generated layouts [here](https://github.com/AprilRobotics/apriltag-imgs). We recommend using the tagStandard41h12 layout.

[![Build Status](https://travis-ci.com/AprilRobotics/apriltag.svg?branch=master)](https://travis-ci.com/AprilRobotics/apriltag)

Papers
======
AprilTag is the subject of the following papers.

[AprilTag: A robust and flexible visual fiducial system](https://april.eecs.umich.edu/papers/details.php?name=olson2011tags)

[AprilTag 2: Efficient and robust fiducial detection](https://april.eecs.umich.edu/papers/details.php?name=wang2016iros)

[Flexible Layouts for Fiducial Tags](https://april.eecs.umich.edu/papers/details.php?name=krogius2019iros)



Usage
=====
[User Guide](https://github.com/AprilRobotics/apriltag/wiki/AprilTag-User-Guide)

Install
=======

 Officially only linux operating systems are supported, although users have had success installing on windows too.
 
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


Flexible Layouts
================
AprilTag 3 supports a wide variety of possible tag layouts in addition to the classic layout supported in AprilTag 2. The tag's data bits can now go outside of the tag border, and it is also possible to define layouts with "holes" inside of the tag border where there are no data bits. In this repo we have included:

* Two families of the new standard layout. This layout adds a layer of data bits around the outside of the tag border, increasing data density, and the number of possible tags, at the cost of a slight decrease in detection distance.
* Two families of circular tags.
* One family which has a hole in the middle. This could be used for example for drone applications by placing different sized tags inside of each other to allow detection over a wide range of distances.

You can generate your own tag families using our other repo, [AprilTag-Generation](https://github.com/AprilRobotics/apriltag-generation).


Support
=======
Please create an issue on this github for any questions instead of sending a private message. This allows other people with the same question to find your answer.


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

