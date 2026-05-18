cmake_minimum_required(VERSION 3.16)

project(cvm_plus_plus LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(cvm
    src/main.cpp
    src/token.cpp
    src/scanner.cpp
    src/parser.cpp
    src/compiler.cpp
    src/vm.cpp
)

target_include_directories(cvm PRIVATE include)
