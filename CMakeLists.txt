cmake_minimum_required(VERSION 3.20)  # GTest import targets introduced in 3.20 are used

project(obc
        VERSION 1.0.0
        DESCRIPTION "An Oberon-07 to LLVM-IR compiler"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)  # Uses C++ 20 Designated Initializers feature
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Dependencies
include(FetchContent)

FetchContent_Declare(CLI11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11
        GIT_TAG v2.4.1)
FetchContent_Declare(GoogleTest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG v1.15.2)

FetchContent_MakeAvailable(CLI11)
FetchContent_MakeAvailable(GoogleTest)

# The compiler library to be linked to the CLI and the unit tests
add_library(obc_lib STATIC
        src/Token.cpp src/Scanner.cpp src/ErrorInfo.cpp src/TokenUtils.cpp
        src/Parser.cpp
        src/Compiler.cpp)
target_include_directories(obc_lib PUBLIC src)
if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(obc_lib PUBLIC -Wall -Wextra)
endif ()

# The compiler CLI
add_executable(obc
        src/main.cpp)
target_link_libraries(obc PRIVATE obc_lib)
target_include_directories(obc
        PRIVATE ${CLI11_SOURCE_DIR}/include)

# Unit tests
include(GoogleTest)
enable_testing()

add_executable(scanner_test_suite src/test/ScannerTestSuite.cpp)
target_link_libraries(scanner_test_suite PRIVATE GTest::gtest GTest::gtest_main PRIVATE obc_lib)

# To avoid a warning introduced by the new Apple linker shipped with XCode15.
# Details at https://gitlab.kitware.com/cmake/cmake/-/issues/25297
if (CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "15.0")
    target_link_options(scanner_test_suite PRIVATE LINKER:-no_warn_duplicate_libraries)
endif ()

gtest_discover_tests(scanner_test_suite)
