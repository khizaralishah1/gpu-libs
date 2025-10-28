cmake_minimum_required(VERSION 3.10)
project(OpenCLExample)

# find_package(OpenCL REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_definitions(CMAKE_SOURCE_DIRECTORY="${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(libs)
add_subdirectory(apps)