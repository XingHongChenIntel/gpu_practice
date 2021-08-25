cmake_minimum_required(VERSION 3.15)
project(gpu_practice C CXX)
set(CMAKE_CXX_STANDARD 17)

# find the dependency package.
# find L0
find_package(LevelZero QUIET)
set(LevelZero_FOUND OFF)
if (NOT ${LevelZero_FOUND})
    set(ADDITIONAL_PATH "${CMAKE_SOURCE_DIR}/third_party/level-zero-sdk")
    message(STATUS "Level Zero installation not found. Trying ${ADDITIONAL_PATH}")

    if (WIN32)
        set(LIB_NAME ze_loader.lib)
    else ()
        set(LIB_NAME libze_loader.so)
    endif ()
    set(LevelZero_LIBRARIES "${ADDITIONAL_PATH}/lib/x64/${LIB_NAME}")
    set(LevelZero_INCLUDE_DIRS "${ADDITIONAL_PATH}/include/level_zero")
else ()
    message(STATUS "Level Zero installation found")
endif ()
# find opencl
find_package(OpenCL QUIET)
set(OpenCL_FOUND OFF)
if (NOT ${OpenCL_FOUND})
    set(ADDITIONAL_PATH "${CMAKE_SOURCE_DIR}/third_party/opencl-sdk")
    message(STATUS "OpenCL installation not found. Using ${ADDITIONAL_PATH}")

    if (WIN32)
        set(LIB_NAME OpenCL.lib)
    else ()
        set(LIB_NAME libOpenCL.so)
    endif ()
    set(OpenCL_LIBRARIES "${ADDITIONAL_PATH}/lib/x64/${LIB_NAME}")
    set(OpenCL_INCLUDE_DIRS "${ADDITIONAL_PATH}/include")
else ()
    message(STATUS "OpenCL installation found")
endif ()

# add the runtime component
set(TARGET_NAME gpu_practice)
add_executable(${TARGET_NAME} main.cpp)
include_directories(framework)
target_include_directories(${TARGET_NAME} PUBLIC ${LevelZero_INCLUDE_DIRS} ${OpenCL_INCLUDE_DIRS})
target_link_libraries(${TARGET_NAME} PUBLIC ${LevelZero_LIBRARIES} ${OpenCL_LIBRARIES})
add_subdirectory(host_code)

