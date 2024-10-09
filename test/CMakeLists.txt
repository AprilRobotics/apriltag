add_library(getline OBJECT getline.c)

add_executable(test_detection test_detection.c)
target_link_libraries(test_detection ${PROJECT_NAME} getline)

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
