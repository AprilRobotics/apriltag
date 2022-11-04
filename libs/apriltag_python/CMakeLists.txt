## Build Python Wrapper

# Build list of include directives
get_target_property(APRILTAG_INCLUDES_LIST ${PROJECT_NAME}-include INTERFACE_INCLUDE_DIRECTORIES)
set(ADDITIONAL_INCLUDE_DIRECTIVES "")
foreach(ITEM ${APRILTAG_INCLUDES_LIST})
    LIST(APPEND ADDITIONAL_INCLUDE_DIRECTIVES "-I\"${ITEM}\"")
endforeach()

if(BUILD_PYTHON_WRAPPER)
    SET(Python_ADDITIONAL_VERSIONS 3)
    find_package(PythonLibs)
    execute_process(COMMAND which python3 OUTPUT_QUIET RESULT_VARIABLE Python3_NOT_FOUND)
    execute_process(COMMAND python3 -c "import numpy" RESULT_VARIABLE Numpy_NOT_FOUND)
endif(BUILD_PYTHON_WRAPPER)

if (NOT Python3_NOT_FOUND AND NOT Numpy_NOT_FOUND AND PYTHONLIBS_FOUND AND BUILD_PYTHON_WRAPPER)
    # TODO deal with both python2/3
    execute_process(COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/python_build_flags.py OUTPUT_VARIABLE PY_OUT)
    set(PY_VARS CFLAGS LDFLAGS LINKER EXT_SUFFIX)
    cmake_parse_arguments(PY "" "${PY_VARS}" "" ${PY_OUT})
    separate_arguments(PY_CFLAGS)
    list(REMOVE_ITEM PY_CFLAGS -flto)
    separate_arguments(PY_LDFLAGS)

    foreach(X detect py_type)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/apriltag_${X}.docstring.h
        COMMAND < ${CMAKE_CURRENT_SOURCE_DIR}/apriltag_${X}.docstring sed 's/\"/\\\\\"/g\; s/^/\"/\; s/$$/\\\\n\"/\;' > apriltag_${X}.docstring.h
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    endforeach()

    add_custom_command(OUTPUT apriltag_pywrap.o
        COMMAND ${CMAKE_C_COMPILER} ${PY_CFLAGS} -I${CMAKE_CURRENT_BINARY_DIR} ${ADDITIONAL_INCLUDE_DIRECTIVES} -c -o apriltag_pywrap.o ${CMAKE_CURRENT_SOURCE_DIR}/apriltag_pywrap.c
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/apriltag_pywrap.c ${CMAKE_CURRENT_BINARY_DIR}/apriltag_detect.docstring.h ${CMAKE_CURRENT_BINARY_DIR}/apriltag_py_type.docstring.h)
    add_custom_command(OUTPUT apriltag${PY_EXT_SUFFIX}
        COMMAND ${PY_LINKER} ${PY_LDFLAGS} -lm -Wl,-rpath,lib apriltag_pywrap.o "$<TARGET_FILE:apriltag-detector>" "$<TARGET_FILE:apriltag-tags>" -o apriltag${PY_EXT_SUFFIX}
        DEPENDS ${PROJECT_NAME} apriltag_pywrap.o)
    add_custom_target(apriltag_python ALL
        DEPENDS apriltag${PY_EXT_SUFFIX})

execute_process(COMMAND python3 -m site --user-site OUTPUT_VARIABLE PY_DEST)
string(STRIP ${PY_DEST} PY_DEST)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/apriltag${PY_EXT_SUFFIX} DESTINATION ${PY_DEST})
endif (NOT Python3_NOT_FOUND AND NOT Numpy_NOT_FOUND AND PYTHONLIBS_FOUND AND BUILD_PYTHON_WRAPPER)
