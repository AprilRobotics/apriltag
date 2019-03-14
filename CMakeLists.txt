cmake_minimum_required(VERSION 3.1)
project(apriltag)

find_package(Threads REQUIRED)
find_package(OpenCV QUIET)

include_directories(.)
aux_source_directory(common COMMON_SRC)
aux_source_directory(. APRILTAG_SRCS)

# Library
add_library(apriltag SHARED ${APRILTAG_SRCS} ${COMMON_SRC})
target_link_libraries(apriltag Threads::Threads m)
file(GLOB_RECURSE HEADER_FILES RELATIVE ${CMAKE_SOURCE_DIR} *.h)
set_target_properties(apriltag PROPERTIES SOVERSION 3 VERSION 3.0.0)


# Examples
# apriltag_demo
add_executable(apriltag_demo example/apriltag_demo.c)
target_link_libraries(apriltag_demo apriltag)

# opencv_demo
if(OpenCV_FOUND)
    add_executable(opencv_demo example/opencv_demo.cc)
    target_link_libraries(opencv_demo apriltag ${OpenCV_LIBRARIES})
    install(TARGETS opencv_demo RUNTIME DESTINATION bin)
endif(OpenCV_FOUND)

# install library and example programs
install(TARGETS apriltag apriltag_demo
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

# install headers
foreach(HEADER ${HEADER_FILES})
    string(REGEX MATCH "(.*)[/\\]" DIR ${HEADER})
    install(FILES ${HEADER} DESTINATION include/${PROJECT_NAME}/${DIR})
endforeach()

FILE(READ apriltag.pc.in PKGC)
STRING(REGEX REPLACE "^prefix=" "prefix=${CMAKE_INSTALL_PREFIX}" PKGC_CONF "${PKGC}" )
FILE(WRITE ${PROJECT_BINARY_DIR}/apriltag.pc ${PKGC_CONF})
install(FILES "${PROJECT_BINARY_DIR}/apriltag.pc" DESTINATION "lib/pkgconfig/")
