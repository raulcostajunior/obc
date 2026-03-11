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
        URL https://github.com/CLIUtils/CLI11/archive/d9473a486b1ffb9a2949a937f3895c7c055afa59.zip)

FetchContent_Declare(GoogleTest
        URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
        FIND_PACKAGE_ARGS 1.16.0...1.17.0 NAMES GTest)

FetchContent_MakeAvailable(CLI11)
FetchContent_MakeAvailable(GoogleTest)

# The compiler library to be linked to the CLI and the unit tests
add_library(obc_lib STATIC
        src/obc/parser.cpp src/obc/scanner/scanner.cpp)
target_sources(obc_lib PUBLIC
        PUBLIC
        FILE_SET CXX_MODULES
        FILES
        src/obc/compiler.cppm
        src/obc/error_info.cppm
        src/obc/parser.cppm
        src/obc/scanner/scanner.cppm
        src/obc/scanner/token.cppm  # module partition interface unit with implementation inline
        src/obc/scanner/token_utils.cpp  # internal module partition unit
        src/obc/version.cppm)

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

# To avoid a warning introduced by the new Apple linker shipped initially with XCode15.
# The warning reads: "ld: warning: ignoring duplicate libraries: 'lib/libgtest.a'"
# Details at https://gitlab.kitware.com/cmake/cmake/-/issues/25297
if (APPLE)
    target_link_options(scanner_test_suite PRIVATE LINKER:-no_warn_duplicate_libraries)
endif ()

gtest_discover_tests(scanner_test_suite)
