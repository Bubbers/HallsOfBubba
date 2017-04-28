cmake_minimum_required(VERSION 3.0)
project("HallsOfBubba")

# Set flags
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build")
set(OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGLEW_STATIC")

INCLUDE_DIRECTORIES(src)

# Compile BUBBA-3D
add_subdirectory(bubba-3d)
add_subdirectory(src)

