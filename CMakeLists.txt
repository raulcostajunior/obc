cmake_minimum_required(VERSION 3.20)  # GTest import targets introduced in 3.20 are used

# TODO: Extract version from repository and generate _version.hpp file
project(obc
        VERSION 1.0.0
        DESCRIPTION "An Oberon-07 to LLVM-IR compiler"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)  # Uses C++ 20 Designated Initializers feature
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

message(STATUS "Platform being built for: ${CMAKE_HOST_SYSTEM_PROCESSOR}")

# CMAKE_MODULE_PATH must contain the path for the build directory:
# a previously run "conan install" will have generated the "Find*.cmake"
# file(s) responsible(s) for locating the external project dependencies
# in there.
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

find_package(CLI11 REQUIRED)
find_package(GTest REQUIRED)

# The compiler library to be linked to the CLI and the unit tests
add_library(obc_lib STATIC src/Token.cpp src/Scanner.cpp src/ErrorInfo.cpp src/TokenUtils.cpp src/TokenUtils.hpp src/TokenUtils.cpp)

# The compiler CLI
add_executable(obc
        src/main.cpp)
target_link_libraries(obc PRIVATE obc_lib)
target_include_directories(obc
        PRIVATE ${CLI11_INCLUDE_DIR})

# Unit tests
include(GoogleTest)
enable_testing()

add_executable(scanner_test_suite src/test/ScannerTestSuite.cpp)
target_include_directories(scanner_test_suite PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)
target_link_libraries(scanner_test_suite PRIVATE GTest::gtest GTest::gtest_main PRIVATE obc_lib)

gtest_discover_tests(scanner_test_suite)
