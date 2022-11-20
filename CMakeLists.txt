cmake_minimum_required(VERSION 3.17)

# TODO: Extract version from repository
project(oboc
        VERSION 1.0.0
        DESCRIPTION "An Oberon-O compiler targetting Webasm and LLVM IR"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# TODO add directory src and specify target from there.
add_executable(oboc
               src/main.cpp)