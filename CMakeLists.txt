cmake_minimum_required(VERSION 3.16)
project(apriltag VERSION 3.4.3 LANGUAGES C)

if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_EXAMPLES "Build example executables" ON)
option(ASAN "Use AddressSanitizer for debug builds to detect memory issues" OFF)

if (ASAN)
    set(ASAN_FLAGS "\
        -fsanitize=address \
        -fsanitize=bool \
        -fsanitize=bounds \
        -fsanitize=enum \
        -fsanitize=float-cast-overflow \
        -fsanitize=float-divide-by-zero \
        -fsanitize=nonnull-attribute \
        -fsanitize=returns-nonnull-attribute \
        -fsanitize=signed-integer-overflow \
        -fsanitize=undefined \
        -fsanitize=vla-bound \
        -fno-sanitize=alignment \
        -fsanitize=leak \
        -fsanitize=object-size \
    ")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${ASAN_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ASAN_FLAGS}")
endif()

# Set a default build type if none was specified
set(default_build_type "Release")

SET(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(WIN32)
    add_compile_definitions(WIN32_LEAN_AND_MEAN)
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE  STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS  "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Werror)
    add_compile_options(-Wpedantic)
    if(CMAKE_C_COMPILER_ID MATCHES "Clang")
        add_compile_options(
            -Wno-gnu-zero-variadic-macro-arguments
            -Wno-strict-prototypes
            -Wno-static-in-inline
        )
    endif()
    add_compile_options(-Wno-shift-negative-value)
endif()

if(CMAKE_C_COMPILER_ID MATCHES "Clang" AND NOT APPLE AND NOT CMAKE_C_SIMULATE_ID MATCHES "MSVC")
    add_link_options("-Wl,-z,relro,-z,now,-z,defs")
endif()

if(CMAKE_C_COMPILER_ID MATCHES "Clang" AND CMAKE_C_SIMULATE_ID MATCHES "MSVC")
    # error: 'strdup' is deprecated: The POSIX name for this item is deprecated. Instead, use the ISO C and C++ conformant name: _strdup.
    # "strdup" is standard since C23
    add_definitions(-D _CRT_NONSTDC_NO_DEPRECATE)
    # ignore "'fopen' is deprecated" and "'strncpy' is deprecated" warnings
    add_definitions(-D _CRT_SECURE_NO_WARNINGS)
endif()

aux_source_directory(common COMMON_SRC)
set(APRILTAG_SRCS apriltag.c apriltag_pose.c apriltag_quad_thresh.c)

# Library
file(GLOB TAG_FILES ${CMAKE_CURRENT_SOURCE_DIR}/tag*.c)
add_library(${PROJECT_NAME} ${APRILTAG_SRCS} ${COMMON_SRC} ${TAG_FILES})
set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

if (MSVC)
    add_compile_definitions("_CRT_SECURE_NO_WARNINGS")
else()
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads)
endif()

if (UNIX)
    target_link_libraries(${PROJECT_NAME} PUBLIC m)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 3 VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 99)


include(GNUInstallDirs)
target_include_directories(${PROJECT_NAME} PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>/apriltag")


# install header file hierarchy
file(GLOB HEADER_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h common/*.h)
list(REMOVE_ITEM HEADER_FILES apriltag_detect.docstring.h apriltag_py_type.docstring.h)

foreach(HEADER ${HEADER_FILES})
    string(REGEX MATCH "(.*)[/\\]" DIR ${HEADER})
    install(FILES ${HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/${DIR})
endforeach()

# export library
set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(targets_export_name "${PROJECT_NAME}Targets")
set(config_install_dir "${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}/cmake")

# Include module with fuction 'write_basic_package_version_file'
include(CMakePackageConfigHelpers)

# Configure '<PROJECT-NAME>Config.cmake'
# Use variables:
#   * targets_export_name
#   * PROJECT_NAME
configure_package_config_file(
        "CMake/apriltagConfig.cmake.in"
        "${project_config}"
        INSTALL_DESTINATION "${config_install_dir}"
)

# Configure '<PROJECT-NAME>ConfigVersion.cmake'
# Note: PROJECT_VERSION is used as a VERSION
write_basic_package_version_file("${version_config}" COMPATIBILITY SameMajorVersion)


# install library
install(TARGETS ${PROJECT_NAME} EXPORT ${targets_export_name}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        )

install(EXPORT ${targets_export_name}
    NAMESPACE apriltag::
    DESTINATION ${config_install_dir})

install(FILES ${project_config} ${version_config} DESTINATION ${config_install_dir})

export(TARGETS apriltag
    NAMESPACE apriltag::
    FILE ${generated_dir}/${targets_export_name}.cmake)


# install pkgconfig file
configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")


# Python wrapper
option(BUILD_PYTHON_WRAPPER "Builds Python wrapper" ON)

find_package(Python3 QUIET COMPONENTS Development NumPy)

if(BUILD_PYTHON_WRAPPER AND Python3_Development_FOUND AND Python3_NumPy_FOUND)

    include(CMake/vtkEncodeString.cmake)

    foreach(X IN ITEMS detect py_type)
        vtk_encode_string(
            INPUT ${CMAKE_CURRENT_SOURCE_DIR}/apriltag_${X}.docstring
            NAME apriltag_${X}_docstring
        )
    endforeach()
    add_custom_target(apriltag_py_docstrings DEPENDS
        ${PROJECT_BINARY_DIR}/apriltag_detect_docstring.h
        ${PROJECT_BINARY_DIR}/apriltag_py_type_docstring.h
    )

    # set the SOABI manually since renaming the library via OUTPUT_NAME does not work on MSVC
    set(apriltag_py_target "apriltag.${Python3_SOABI}")
    Python3_add_library(${apriltag_py_target} MODULE ${CMAKE_CURRENT_SOURCE_DIR}/apriltag_pywrap.c)
    add_dependencies(${apriltag_py_target} apriltag_py_docstrings)
    target_link_libraries(${apriltag_py_target} PRIVATE apriltag Python3::Python Python3::NumPy)
    target_include_directories(${apriltag_py_target} PRIVATE ${PROJECT_BINARY_DIR})

    set(PY_DEST ${CMAKE_INSTALL_PREFIX}/lib/python${Python3_VERSION_MAJOR}.${Python3_VERSION_MINOR}/site-packages/)
    install(TARGETS ${apriltag_py_target} LIBRARY DESTINATION ${PY_DEST})
elseif(BUILD_PYTHON_WRAPPER)
    message(WARNING
        "Python bindings requested (BUILD_PYTHON_WRAPPER=ON) but Development and NumPy not found. "
        "Python bindings will not be built. Set BUILD_PYTHON_WRAPPER=OFF to silent this warnings."
     )
endif()

# Examples
if (BUILD_EXAMPLES)
    # apriltag_demo
    add_executable(apriltag_demo example/apriltag_demo.c)
    target_link_libraries(apriltag_demo ${PROJECT_NAME})

    # opencv_demo
    set(_OpenCV_REQUIRED_COMPONENTS core imgproc videoio highgui)
    find_package(OpenCV COMPONENTS ${_OpenCV_REQUIRED_COMPONENTS} QUIET CONFIG)
    if(OpenCV_FOUND)
        enable_language(CXX)
        # NB: contrib required for TickMeter in OpenCV 2.4. This is only required for 16.04 backwards compatibility and can be removed in the future.
        #     If we add it to the find_package initially, the demo won't build for newer OpenCV versions
        if(OpenCV_VERSION VERSION_LESS "3.0.0")
            list(APPEND _OpenCV_REQUIRED_COMPONENTS contrib)
            find_package(OpenCV COMPONENTS ${_OpenCV_REQUIRED_COMPONENTS} CONFIG)
        endif()

        add_executable(opencv_demo example/opencv_demo.cc)
        target_link_libraries(opencv_demo apriltag ${OpenCV_LIBRARIES})
        set_target_properties(opencv_demo PROPERTIES CXX_STANDARD 11)
        install(TARGETS opencv_demo RUNTIME DESTINATION bin)
    else()
        message(STATUS "OpenCV not found: Not building demo")
    endif(OpenCV_FOUND)

    # install example programs
    install(TARGETS apriltag_demo RUNTIME DESTINATION bin)
endif()

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()
