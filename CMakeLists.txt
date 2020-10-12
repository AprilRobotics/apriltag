cmake_minimum_required(VERSION 3.1)
project(apriltag)

find_package(OpenCV QUIET)

include_directories(.)
aux_source_directory(common COMMON_SRC)
set(APRILTAG_SRCS apriltag.c apriltag_pose.c apriltag_quad_thresh.c)

set(CMAKE_BUILD_TYPE Release)

# Library
file(GLOB TAG_FILES ${CMAKE_SOURCE_DIR}/tag*.c)
set_source_files_properties(SOURCE ${TAG_FILES} PROPERTIES COMPILE_FLAGS -O0)
add_library(${PROJECT_NAME} SHARED ${APRILTAG_SRCS} ${COMMON_SRC} ${TAG_FILES})
if (MSVC)
    # FindThreads will not find pthread.h with MSVC
    # winmm is necessary for __imp_timeGetTime
    find_library(PTHREAD_LIBRARIES NAMES pthreads)
    target_link_libraries(${PROJECT_NAME} ${PTHREAD_LIBRARIES} winmm)
else()
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads m)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 3 VERSION 3.1.0)

include(GNUInstallDirs)
target_include_directories(${PROJECT_NAME} PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/common>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>/apriltag")

# install library
install(TARGETS ${PROJECT_NAME} EXPORT apriltagTargets
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
install(EXPORT apriltagTargets
    FILE apriltagTargets.cmake
    NAMESPACE apriltag::
    DESTINATION share/apriltag/cmake
)
install(FILES CMake/apriltagConfig.cmake DESTINATION share/apriltag/cmake)
export(TARGETS apriltag
    NAMESPACE apriltag::
    FILE apriltagTargets.cmake)

FILE(READ apriltag.pc.in PKGC)
STRING(REGEX REPLACE "^prefix=" "prefix=${CMAKE_INSTALL_PREFIX}" PKGC_CONF "${PKGC}" )
FILE(WRITE ${PROJECT_BINARY_DIR}/apriltag.pc ${PKGC_CONF})
install(FILES "${PROJECT_BINARY_DIR}/apriltag.pc" DESTINATION "lib/pkgconfig/")


# Python wrapper
SET(Python_ADDITIONAL_VERSIONS 3)
find_package(PythonLibs)
execute_process(COMMAND which python3 OUTPUT_QUIET RESULT_VARIABLE Python3_NOT_FOUND)
execute_process(COMMAND python3 -c "import numpy" RESULT_VARIABLE Numpy_NOT_FOUND)
if (NOT Python3_NOT_FOUND AND NOT Numpy_NOT_FOUND AND PYTHONLIBS_FOUND)
# TODO deal with both python2/3
execute_process(COMMAND python3 ${CMAKE_SOURCE_DIR}/python_build_flags.py OUTPUT_VARIABLE PY_OUT)
set(PY_VARS CFLAGS LDFLAGS LINKER EXT_SUFFIX)
cmake_parse_arguments(PY "" "${PY_VARS}" "" ${PY_OUT})
separate_arguments(PY_CFLAGS)
separate_arguments(PY_LDFLAGS)

foreach(X detect py_type)
add_custom_command(OUTPUT ${PROJECT_BINARY_DIR}/apriltag_${X}.docstring.h
    COMMAND < ${CMAKE_SOURCE_DIR}/apriltag_${X}.docstring sed 's/\"/\\\\\"/g\; s/^/\"/\; s/$$/\\\\n\"/\;' > apriltag_${X}.docstring.h
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
endforeach()

add_custom_command(OUTPUT apriltag_pywrap.o
    COMMAND ${CMAKE_C_COMPILER} ${PY_CFLAGS} -I${PROJECT_BINARY_DIR} -c -o apriltag_pywrap.o ${CMAKE_SOURCE_DIR}/apriltag_pywrap.c
    DEPENDS ${CMAKE_SOURCE_DIR}/apriltag_pywrap.c ${PROJECT_BINARY_DIR}/apriltag_detect.docstring.h ${PROJECT_BINARY_DIR}/apriltag_py_type.docstring.h)
add_custom_command(OUTPUT apriltag${PY_EXT_SUFFIX}
    COMMAND ${PY_LINKER} ${PY_LDFLAGS} -Wl,-rpath,lib apriltag_pywrap.o $<TARGET_FILE:apriltag> -o apriltag${PY_EXT_SUFFIX}
    DEPENDS ${PROJECT_NAME} apriltag_pywrap.o)
add_custom_target(apriltag_python ALL
    DEPENDS apriltag${PY_EXT_SUFFIX})

execute_process(COMMAND python3 -m site --user-site OUTPUT_VARIABLE PY_DEST)
string(STRIP ${PY_DEST} PY_DEST)
install(CODE "execute_process(COMMAND cp ${PROJECT_BINARY_DIR}/apriltag${PY_EXT_SUFFIX} ${PY_DEST})")
endif (NOT Python3_NOT_FOUND AND NOT Numpy_NOT_FOUND AND PYTHONLIBS_FOUND)


# Examples
# apriltag_demo
add_executable(apriltag_demo example/apriltag_demo.c)
target_link_libraries(apriltag_demo apriltag)

# opencv_demo
if(OpenCV_FOUND)
    add_executable(opencv_demo example/opencv_demo.cc)
    target_link_libraries(opencv_demo apriltag ${OpenCV_LIBRARIES})
    set_target_properties(opencv_demo PROPERTIES CXX_STANDARD 11)
    install(TARGETS opencv_demo RUNTIME DESTINATION bin)
endif(OpenCV_FOUND)

# install example programs
install(TARGETS apriltag_demo RUNTIME DESTINATION bin)

