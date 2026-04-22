cmake_minimum_required(VERSION 3.10)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins)

project(raytracer CXX)

add_executable(raytracer
    project/src/main.cpp
    project/src/security/args.cpp
    project/src/math/Vector3D.cpp
    project/src/math/Point3D.cpp
    project/src/core/Ray.cpp
    project/src/render/Rectangle3D.cpp
    project/src/render/Camera.cpp
)

target_include_directories(raytracer
    PUBLIC
        project/includes
        project/includes/rt
        project/includes/security
        project/includes/Error
)

target_compile_options(raytracer
    PRIVATE
        -Wall
        -Wextra
        -Werror
)
