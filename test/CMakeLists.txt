add_library(getline OBJECT getline.c)

add_executable(test_detection test_detection.c)
target_link_libraries(test_detection ${PROJECT_NAME} getline)

# Add the pose estimation test executable
add_executable(test_tag_pose_estimation test_tag_pose_estimation.c)
target_link_libraries(test_tag_pose_estimation ${PROJECT_NAME} getline)

# test images with true detection
set(TEST_IMAGE_NAMES
    "33369213973_9d9bb4cc96_c"
    "34085369442_304b6bafd9_c"
    "34139872896_defdb2f8d9_c"
)

foreach(IMG IN LISTS TEST_IMAGE_NAMES)
    add_test(NAME test_detection_${IMG}
             COMMAND $<TARGET_FILE:test_detection> data/${IMG}
             WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endforeach()

# Add pose estimation tests for each image
foreach(IMG IN LISTS TEST_IMAGE_NAMES)
    add_test(NAME test_tag_pose_estimation_${IMG}
             COMMAND $<TARGET_FILE:test_tag_pose_estimation> 
             data/${IMG}.jpg data/${IMG}.txt
             WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endforeach()

# Quick decode test
file(GLOB COMMON_SRC "${CMAKE_SOURCE_DIR}/common/*.c")
file(GLOB TAG_FILES "${CMAKE_SOURCE_DIR}/tag*.c")

add_executable(test_quick_decode test_quick_decode.c "${CMAKE_SOURCE_DIR}/apriltag_quad_thresh.c" ${COMMON_SRC} ${TAG_FILES})
target_include_directories(test_quick_decode PRIVATE "${CMAKE_SOURCE_DIR}")

if (UNIX)
    target_link_libraries(test_quick_decode m)
endif()

if(NOT MSVC)
    find_package(Threads REQUIRED)
    target_link_libraries(test_quick_decode Threads::Threads)
endif()

add_test(NAME test_quick_decode COMMAND test_quick_decode)

