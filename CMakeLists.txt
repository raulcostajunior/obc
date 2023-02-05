cmake_minimum_required(VERSION 3.17)

# TODO: Extract version from repository and generate _version.hpp file
project(obc
        VERSION 1.0.0
        DESCRIPTION "An Oberon-0 to LLVM-IR compiler"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# CMAKE_MODULE_PATH must contain the path for the build directory:
# a previously run "conan install" will have generated the "Find*.cmake"
# file(s) responsible(s) for locating the external project dependencies
# in there.
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

find_package(CLI11 REQUIRED)
find_package(GTest REQUIRED)

# TODO add directory src and specify target from there.
add_executable(obc
               src/main.cpp src/token.cpp src/scanner.cpp)

target_include_directories(obc
        PRIVATE ${CLI11_INCLUDE_DIR})
