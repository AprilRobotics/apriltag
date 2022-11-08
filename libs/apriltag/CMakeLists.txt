include(GNUInstallDirs)
include(GenerateExportHeader)
include(CMake/apriltagHelperFunctions.cmake)

### Build Apriltag Libraries
## The following targets are built
##      apriltag-detector (Apriltag's core functionality with only API functions exported)
##      apriltag-tags (Apriltag's tag libraries)
##      apriltag-<tag_family_name> e.g apriltag-tagStandard41h12 (Individual libraries for each tag family)
##      apriltag-utils (Utility functions used in the examples)
##      apriltag *(Legacy build with all functionality (detector, tags, utils) exported)

### Notes
# CMake Object Libraries are used where possible to minimize repeat compilation of common code
# *except* where object libraries would break the export definitions, especially on Windows 
# (https://gitlab.kitware.com/cmake/cmake/-/issues/20057)
# CMake Interface targets are used to share properties with other targets

set(HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(HEADERS_COMMON_DIR ${HEADERS_DIR}/apriltag/common)

# Common Souce

set(HEADERS_COMMON
    ${HEADERS_COMMON_DIR}/debug_print.h
    ${HEADERS_COMMON_DIR}/image_types.h
    ${HEADERS_COMMON_DIR}/image_u8.h
    ${HEADERS_COMMON_DIR}/image_u8x3.h
    ${HEADERS_COMMON_DIR}/math_util.h
    ${HEADERS_COMMON_DIR}/pnm.h
    ${HEADERS_COMMON_DIR}/time_util.h
    ${HEADERS_COMMON_DIR}/zarray.h
)

set(SRC_COMMON
    src/common/image_u8.c
    src/common/image_u8x3.c
    src/common/pnm.c
    src/common/time_util.c
)

add_library(common OBJECT  ${SRC_COMMON} ${HEADERS_COMMON})

set_target_properties(common PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_apriltag_export_all(common)
target_include_directories(common 
    PUBLIC
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>"
        "$<BUILD_INTERFACE:${HEADERS_DIR}>"
)

# includes interface target 
# used to overcome object libraries not passing on their include_dirs
add_library(${PROJECT_NAME}-include INTERFACE)

target_include_directories(${PROJECT_NAME}-include
   INTERFACE
   "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>/${PROJECT_NAME}"
   "$<BUILD_INTERFACE:${HEADERS_DIR}>"
   # include cmake-generated apriltag_export.h
   "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>"
)

set_target_properties(${PROJECT_NAME}-include PROPERTIES EXPORT_NAME includes)

# Use interface targets to manage dependencies for different targets
add_library(${PROJECT_NAME}-link-libs INTERFACE)
add_library(${PROJECT_NAME}-utils-link-libs INTERFACE)
if (MSVC)
    add_compile_definitions("_CRT_SECURE_NO_WARNINGS")
else()
    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME}-link-libs INTERFACE Threads::Threads m)
    target_link_libraries(${PROJECT_NAME}-utils-link-libs INTERFACE m)
endif()

# Detector Library
set(HEADERS_DETECTOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/apriltag)

set(HEADERS_DETECTOR
    ${HEADERS_DETECTOR_DIR}/apriltag.h
    ${HEADERS_DETECTOR_DIR}/apriltag_pose.h
    ${HEADERS_DETECTOR_DIR}/common/g2d.h
    ${HEADERS_DETECTOR_DIR}/common/homography.h
    ${HEADERS_DETECTOR_DIR}/common/matd.h
    ${HEADERS_DETECTOR_DIR}/common/postscript_utils.h
    ${HEADERS_DETECTOR_DIR}/common/pthreads_cross.h
    ${HEADERS_DETECTOR_DIR}/common/svd22.h
    ${HEADERS_DETECTOR_DIR}/common/timeprofile.h
    ${HEADERS_DETECTOR_DIR}/common/unionfind.h
    ${HEADERS_DETECTOR_DIR}/common/workerpool.h
    ${HEADERS_DETECTOR_DIR}/common/zmaxheap.h
)

SET(SRC_DETECTOR
    src/apriltag.c 
    src/apriltag_pose.c 
    src/apriltag_quad_thresh.c
    src/common/g2d.c
    src/common/homography.c
    src/common/matd.c
    src/common/pthreads_cross.cpp
    src/common/svd22.c
    src/common/workerpool.c
    src/common/zmaxheap.c
    ${HEADERS_DETECTOR}
)

add_library(${PROJECT_NAME}-detector 
    ${SRC_COMMON}
    ${SRC_DETECTOR}
    ${HEADERS_COMMON}
    ${HEADERS_DETECTOR}
)

target_link_libraries(${PROJECT_NAME}-detector 
    PUBLIC
        ${PROJECT_NAME}-include 
        ${PROJECT_NAME}-link-libs
)

set_apriltag_lib_property_defaults(${PROJECT_NAME}-detector)
set_apriltag_named_exports_only(${PROJECT_NAME}-detector)
set_target_properties(${PROJECT_NAME}-detector PROPERTIES EXPORT_NAME detector)
# auto generate headers used for all libs here - ensure this call after
# set_apriltag_named_exports_only() as that fn overrides DEFINE_SYMBOL
generate_export_header(${PROJECT_NAME}-detector BASE_NAME apriltag)

# Tag Family Libraries
set(TAG_FAMILIES
    tag16h5
    tag25h9
    tag36h10
    tag36h11
    tagCircle21h7
    tagCircle49h12
    tagCustom48h12
    tagStandard41h12
    tagStandard52h13
)

set(TARGETS_TAGS "")
set(HEADERS_TAGS "")
set(SRC_TAGS "")

foreach(TAG_FAMILY ${TAG_FAMILIES})
    # for each tag family
    set(TAG_FAMILY_HEADER ${HEADERS_DETECTOR_DIR}/${TAG_FAMILY}.h)
    set(TAG_FAMILY_SRC src/${TAG_FAMILY}.c)

    set(TAG_TARGET ${PROJECT_NAME}-${TAG_FAMILY})

    # create Tag-Family object target
    add_library(${TAG_TARGET} ${TAG_FAMILY_SRC} ${TAG_FAMILY_HEADER})
    set_apriltag_lib_property_defaults(${TAG_TARGET})
    set_apriltag_named_exports_only(${TAG_TARGET})
    set_target_properties(${TAG_TARGET} PROPERTIES EXPORT_NAME ${TAG_FAMILY})
    target_link_libraries(${TAG_TARGET} PUBLIC ${PROJECT_NAME}-include)

    LIST(APPEND TARGETS_TAGS ${TAG_TARGET})
    LIST(APPEND HEADERS_TAGS ${TAG_FAMILY_HEADER})
    LIST(APPEND SRC_TAGS ${TAG_FAMILY_SRC})
endforeach()

# All-Tag-Families library
add_library(${PROJECT_NAME}-tags ${SRC_TAGS} ${SRC_HEADERS})

set_target_properties(${PROJECT_NAME}-tags PROPERTIES EXPORT_NAME tags)
set_apriltag_lib_property_defaults(${PROJECT_NAME}-tags)
set_apriltag_named_exports_only(${PROJECT_NAME}-tags)
target_link_libraries(${PROJECT_NAME}-tags PUBLIC ${PROJECT_NAME}-include)

# Utils Library
add_library(utils-object OBJECT)

set_target_properties(utils-object PROPERTIES POSITION_INDEPENDENT_CODE ON)
set_apriltag_export_all(utils-object)
target_include_directories(utils-object PRIVATE "$<TARGET_PROPERTY:common,INTERFACE_INCLUDE_DIRECTORIES>")

set(HEADERS_UTIL
    ${HEADERS_COMMON_DIR}/getopt.h
    ${HEADERS_COMMON_DIR}/image_u8x4.h
    ${HEADERS_COMMON_DIR}/pam.h
    ${HEADERS_COMMON_DIR}/pjpeg.h
    ${HEADERS_COMMON_DIR}/string_util.h
    ${HEADERS_COMMON_DIR}/zhash.h
)

target_sources(utils-object 
    PRIVATE
        src/common/string_util.c
        src/common/pjpeg-idct.c
        src/common/pjpeg.c
        src/common/image_u8x4.c
        src/common/getopt.c
        src/common/pam.c
        src/common/zhash.c
        src/common/zarray.c
        ${HEADERS_UTIL}
)

add_library(${PROJECT_NAME}-utils "$<TARGET_OBJECTS:utils-object>" "$<TARGET_OBJECTS:common>")

set_apriltag_lib_property_defaults(${PROJECT_NAME}-utils)
set_apriltag_export_all(${PROJECT_NAME}-utils)
set_target_properties(${PROJECT_NAME}-utils PROPERTIES EXPORT_NAME utils)
target_link_libraries(${PROJECT_NAME}-utils 
    PUBLIC 
        ${PROJECT_NAME}-include
        ${PROJECT_NAME}-utils-link-libs
)

# install library header file hierarchy
install(DIRECTORY ${HEADERS_DIR}/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#install apriltag_exports.h
install(FILES "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/apriltag_export.h>" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}/)

# Combined Apriltag Library to support legacy builds
add_library(${PROJECT_NAME} 
        ${SRC_DETECTOR}
        ${SRC_TAGS}
        ${HEADERS_DETECTOR}
        ${HEADERS_TAGS}
        "$<TARGET_OBJECTS:utils-object>"
        "$<TARGET_OBJECTS:common>"
)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC
    ${PROJECT_NAME}-include
    ${PROJECT_NAME}-link-libs
    ${PROJECT_NAME}-utils-link-libs
)

set_apriltag_lib_property_defaults(${PROJECT_NAME})
set_apriltag_export_all(${PROJECT_NAME})

# export library
set(generated_dir ${CMAKE_CURRENT_BINARY_DIR}/generated)
set(version_config ${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake)
set(project_config ${generated_dir}/${PROJECT_NAME}Config.cmake)
set(targets_export_name ${PROJECT_NAME}Targets)
set(config_install_dir share/${PROJECT_NAME}/cmake)

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


set(APRILTAG_INSTALL_TARGETS
    ${PROJECT_NAME}-include
    ${PROJECT_NAME}-link-libs
    ${PROJECT_NAME}-utils-link-libs
    ${PROJECT_NAME}-detector
    ${PROJECT_NAME}-tags
    ${TARGETS_TAGS}
    ${PROJECT_NAME}-utils
    ${PROJECT_NAME}
)

# install library
install(TARGETS ${APRILTAG_INSTALL_TARGETS} EXPORT ${targets_export_name}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT ${targets_export_name}
    NAMESPACE apriltag::
    DESTINATION ${config_install_dir})

install(FILES ${project_config} ${version_config} DESTINATION ${config_install_dir})

export(TARGETS ${APRILTAG_INSTALL_TARGETS}
    NAMESPACE apriltag::
    FILE ${generated_dir}/${targets_export_name}.cmake
)

# install pkgconfig file
configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)