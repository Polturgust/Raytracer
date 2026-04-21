cmake_minimum_required(VERSION 3.10)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins)

project(raytracer CXX)

add_executable(raytracer
    project/src/main.cpp
)

target_include_directories(raytracer
    PUBLIC
        project/includes
)

target_compile_options(raytracer
    PRIVATE
        -Wall
        -Wextra
        -Werror
)
