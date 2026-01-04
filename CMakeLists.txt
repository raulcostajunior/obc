cmake_minimum_required(VERSION 3.28)

project(obc
        VERSION 1.0.0
        DESCRIPTION "An Oberon-07 to LLVM-IR compiler"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Dependencies
include(FetchContent)

FetchContent_Declare(CLI11
        GIT_REPOSITORY https://github.com/CLIUtils/CLI11
        GIT_TAG d9473a486b1ffb9a2949a937f3895c7c055afa59)
FetchContent_Declare(GoogleTest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG v1.15.2
        FIND_PACKAGE_ARGS 1.15.2...1.16.0 NAMES GTest)

FetchContent_MakeAvailable(CLI11)
FetchContent_MakeAvailable(GoogleTest)

# The compiler library to be linked to the CLI and the unit tests
add_library(obc_lib STATIC
        src/error_info.cpp src/parser.cpp src/scanner.cpp)
target_sources(obc_lib PUBLIC
        PUBLIC
        FILE_SET CXX_MODULES
        FILES
        ${PROJECT_SOURCE_DIR}/src/compiler.cppm
        ${PROJECT_SOURCE_DIR}/src/error_info.cppm
        ${PROJECT_SOURCE_DIR}/src/parser.cppm
        ${PROJECT_SOURCE_DIR}/src/scanner.cppm
        ${PROJECT_SOURCE_DIR}/src/scanner.token.cppm  # module partition interface with implementation inline
        ${PROJECT_SOURCE_DIR}/src/scanner.token_utils.cpp  # internal module partition implementation
        ${PROJECT_SOURCE_DIR}/src/version.cppm)

# The compiler CLI
add_executable(obc src/main.cpp)
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
if ((CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang") AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "15.0")
    target_link_options(scanner_test_suite PRIVATE LINKER:-no_warn_duplicate_libraries)
endif ()

gtest_discover_tests(scanner_test_suite)
