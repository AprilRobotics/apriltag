## Build Examples

### Note
# apriltag targets are in the same build tree so can be used directly without installation 
# or including the build-generated apriltagsTargets.cmake file 
#
# Typical usage would be to build and *install* the apriltag libraries and then, for your project use
#       find_package(apriltag REQUIRED)
#       target_link_libraries(my_application PRIVATE apriltag::detector apriltag::utils apriltag::tags)

# apriltag_demo
add_executable(apriltag_demo apriltag_demo.c)
target_link_libraries(apriltag_demo PRIVATE apriltag-detector apriltag-utils apriltag-tags)

set(_OpenCV_REQUIRED_COMPONENTS core imgproc videoio highgui)
find_package(OpenCV COMPONENTS ${_OpenCV_REQUIRED_COMPONENTS} QUIET CONFIG)
if(OpenCV_FOUND)
    # NB: contrib required for TickMeter in OpenCV 2.4. This is only required for 16.04 backwards compatibility and can be removed in the future.
    #     If we add it to the find_package initially, the demo won't build for newer OpenCV versions
    if(OpenCV_VERSION VERSION_LESS "3.0.0")
        list(APPEND _OpenCV_REQUIRED_COMPONENTS contrib)
        find_package(OpenCV COMPONENTS ${_OpenCV_REQUIRED_COMPONENTS} CONFIG)
    endif()

    add_executable(opencv_demo opencv_demo.cc)
    target_link_libraries(opencv_demo apriltag-detector apriltag-tags apriltag-utils ${OpenCV_LIBRARIES})
    set_target_properties(opencv_demo PROPERTIES CXX_STANDARD 11)
    install(TARGETS opencv_demo RUNTIME DESTINATION bin)
else()
    message(STATUS "OpenCV not found: Not building demo")
endif(OpenCV_FOUND)

# install example programs
install(TARGETS apriltag_demo RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})