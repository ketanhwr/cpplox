cmake_minimum_required(VERSION 3.15)

project(cpplox LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(cpplox
    src/main.cpp
    src/scanner.cpp
    src/token.cpp
)

target_include_directories(cpplox
    PUBLIC inc
)

