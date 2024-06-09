cmake_minimum_required(VERSION 3.15)

project(cpplox LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Debug)

############################
## Generate AST definition
############################

find_package(Python COMPONENTS Interpreter Development)

set(generated_dir ${PROJECT_BINARY_DIR}/generated)

file(MAKE_DIRECTORY "${generated_dir}")

add_custom_command(
    OUTPUT "${generated_dir}/expr.hpp"
    COMMAND "${Python_EXECUTABLE}"
        "${PROJECT_SOURCE_DIR}/tools/generate_ast.py"
        "${generated_dir}"
    DEPENDS "${PROJECT_SOURCE_DIR}/tools/generate_ast.py"
    COMMENT "Generating AST Structures"
)

############################

add_executable(cpplox
    src/main.cpp
    src/scanner.cpp
    src/token.cpp
    src/value.cpp
    src/ast_printer.cpp
    src/parser.cpp
    "${generated_dir}/expr.hpp"
)

target_include_directories(cpplox PUBLIC
    inc
    "${generated_dir}"
)

