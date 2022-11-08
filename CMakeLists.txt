cmake_minimum_required(VERSION 3.5)
project(apriltag VERSION 3.3.0 LANGUAGES C CXX)

if (MSVC)
    add_compile_definitions("_CRT_SECURE_NO_WARNINGS")
endif()

if(POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()

option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_EXAMPLES "Build example binaries" ON)
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

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE  STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS  "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# build main library
add_subdirectory(libs/apriltag)

## Python wrapper
include(CMakeDependentOption)
cmake_dependent_option(BUILD_PYTHON_WRAPPER "Builds Python wrapper" ON BUILD_SHARED_LIBS OFF)

if(BUILD_PYTHON_WRAPPER)
    # build python wrapper
    add_subdirectory(libs/apriltag_python)
endif()

# Examples
if(BUILD_EXAMPLES)
    # build examples
    add_subdirectory(example)
endif()