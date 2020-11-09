include("${CMAKE_CURRENT_LIST_DIR}/apriltagTargets.cmake")

if (NOT MSVC) 
    include(CMakeFindDependencyMacro)
    find_dependency(Threads)
endif()

