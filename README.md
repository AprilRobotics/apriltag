AprilTag 3
==========
AprilTag is a visual fiducial system popular in robotics research. This repository contains the most recent version of AprilTag, AprilTag 3, which includes a faster (>2x) detector, improved detection rate on small tags, flexible tag layouts, and pose estimation. AprilTag consists of a small C library with minimal dependencies.

You can find tag images for the pre-generated layouts [here](https://github.com/AprilRobotics/apriltag-imgs). We recommend using the tagStandard41h12 layout.

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

Officially only Linux operating systems are supported, although users have had success installing on Windows too.

The default installation will place headers in /usr/local/include and shared library in /usr/local/lib. It also installs a pkg-config script into /usr/local/lib/pkgconfig and will install a python wrapper if python3 is installed.

## cmake
If you have CMake installed, then do:
```
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --target install
```
This will build shared (\*.so) libraries by default. If you need static (\*.a) libraries set `BUILD_SHARED_LIBS` to `OFF`:
```
cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF
cmake --build build --target install
```

If you have Ninja (`sudo apt install ninja-build`) installed, you can use:
```
cmake -B build -GNinja -DCMAKE_BUILD_TYPE=Release
cmake --build build --target install
```
to generate and compile via the ninja build script. It will be much faster than with cmake's default Makefile generator.

You can omit `--target install` if you only want to use this locally without installing.

## make
Otherwise, we have a handwritten makefile you can use (be warned it will do slightly different things):
```
make -j
sudo make install
```

To install to a different directory than /usr/local:

    $ PREFIX=/some/path sudo make install

Debugging
=========

You can enable [AddressSanitizer](https://clang.llvm.org/docs/AddressSanitizer.html) to debug memory issues for Debug builds by setting the `ASAN` option:
```
cmake -B build -GNinja -DCMAKE_BUILD_TYPE=Debug -DASAN=ON
cmake --build build
```

Mostly you can then run your executables as usual and inspect the sanitiser output. If you get a message like `ASan runtime does not come first in initial library list; you should either link runtime to your application or manually preload it with LD_PRELOAD.` you have to preload the corresponding `libasan.so.5` like this:
```
LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libasan.so.5 ./build/opencv_demo
```

Flexible Layouts
================
AprilTag 3 supports a wide variety of possible tag layouts in addition to the classic layout supported in AprilTag 2. The tag's data bits can now go outside of the tag border, and it is also possible to define layouts with "holes" inside of the tag border where there are no data bits. In this repo we have included:

* Two families of the new standard layout. This layout adds a layer of data bits around the outside of the tag border, increasing data density, and the number of possible tags, at the cost of a slight decrease in detection distance.
* Two families of circular tags.
* One family which has a hole in the middle. This could be used for example for drone applications by placing different sized tags inside of each other to allow detection over a wide range of distances.

You can generate your own tag families using our other repo, [AprilTag-Generation](https://github.com/AprilRobotics/apriltag-generation).


Support
=======
Please create an issue on this GitHub for any questions instead of sending a private message. This allows other people with the same question to find your answer.
