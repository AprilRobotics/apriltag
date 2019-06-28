cmake_minimum_required(VERSION 3.1)
project(apriltag)

find_package(OpenCV QUIET)

include_directories(.)
aux_source_directory(common COMMON_SRC)
set(APRILTAG_SRCS apriltag.c apriltag_pose.c apriltag_quad_thresh.c)

set(CMAKE_BUILD_TYPE Release)

# Library
add_library(${PROJECT_NAME} SHARED ${APRILTAG_SRCS} ${COMMON_SRC})
if (MSVC)
    # FindThreads will not find pthread.h with MSVC
    # winmm is necessary for __imp_timeGetTime
    find_library(PTHREAD_LIBRARIES NAMES pthreads)
    target_link_libraries(${PROJECT_NAME} ${PTHREAD_LIBRARIES} winmm)
else()
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} Threads::Threads m)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 3 VERSION 3.1.0)

include(GNUInstallDirs)
target_include_directories(${PROJECT_NAME} PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/common>"
    "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>/apriltag")

set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_INSTALL_LIBDIR}
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_INSTALL_LIBDIR}
)

file(GLOB TAG_FILES ${CMAKE_SOURCE_DIR}/tag*.c)
set_source_files_properties(SOURCE ${TAG_FILES} PROPERTIES COMPILE_FLAGS -O0)

# install library
install(TARGETS ${PROJECT_NAME} EXPORT apriltag
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# install header file hierarchy
file(GLOB_RECURSE HEADER_FILES RELATIVE ${CMAKE_SOURCE_DIR} *.h)
foreach(HEADER ${HEADER_FILES})
    string(REGEX MATCH "(.*)[/\\]" DIR ${HEADER})
    install(FILES ${HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${DIR})
endforeach()

# export library
install(EXPORT apriltag
    DESTINATION share/apriltag/cmake
    FILE apriltagConfig.cmake
)

FILE(READ apriltag.pc.in PKGC)
STRING(REGEX REPLACE "^prefix=" "prefix=${CMAKE_INSTALL_PREFIX}" PKGC_CONF "${PKGC}" )
FILE(WRITE ${PROJECT_BINARY_DIR}/apriltag.pc ${PKGC_CONF})
install(FILES "${PROJECT_BINARY_DIR}/apriltag.pc" DESTINATION "lib/pkgconfig/")


# Python wrapper
message("Starting py wrapper")
# TODO deal with both python2/3
execute_process(COMMAND python ${CMAKE_SOURCE_DIR}/python_build_flags.py OUTPUT_VARIABLE PY_OUT)
message("PY_OUT: " ${PY_OUT})
list(LENGTH PY_OUT PY_OUT_LEN)
message("LEN: " ${PY_OUT_LEN})
set(PY_VARS CFLAGS LDFLAGS LINKER EXT_SUFFIX)
cmake_parse_arguments(PY "" "${PY_VARS}" "" ${PY_OUT})
message("PY_CFLAGS: " ${PY_CFLAGS})
message("PY_LDFLAGS: " ${PY_LDFLAGS})
message("PY_LINKER: " ${PY_LINKER})
message("PY_EXT_SUFFIX: " ${PY_EXT_SUFFIX})

#add_library(apriltag_python apriltag_pywrap.c)
set_source_files_properties(SOURCE ${CMAKE_SOURCE_DIR}/apriltag_pywrap.c PROPERTIES COMPILE_FLAGS "${PY_CFLAGS}")

#configure_file(${CMAKE_SOURCE_DIR}/apriltag_detect.docstring ${PROJECT_BINARY_DIR}/apriltag_detect.docstring.h COPYONLY)
foreach(X detect py_type)
set_source_files_properties(SOURCE ${CMAKE_SOURCE_DIR}/apriltag_pywrap.c PROPERTIES OBJECT_DEPENDS ${PROJECT_BINARY_DIR}/apriltag_${X}.docstring.h)
add_custom_command(OUTPUT ${PROJECT_BINARY_DIR}/apriltag_${X}.docstring.h
    COMMAND < ${CMAKE_SOURCE_DIR}/apriltag_${X}.docstring sed 's/\"/\\\\\"/g\; s/^/\"/\; s/$$/\\\\n\"/\;' > apriltag_${X}.docstring.h
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
endforeach()
#target_include_directories(apriltag_python PUBLIC ${PROJECT_BINARY_DIR})


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

# install example programs
install(TARGETS apriltag_demo RUNTIME DESTINATION bin)

