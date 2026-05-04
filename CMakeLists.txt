cmake_minimum_required(VERSION 3.10)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins)

project(raytracer CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCONFIG REQUIRED libconfig++)

file(GLOB_RECURSE PROJECT_SRCS
    ${CMAKE_SOURCE_DIR}/project/src/*.cpp
)

add_executable(raytracer ${PROJECT_SRCS})

target_include_directories(raytracer
    PUBLIC
        project/includes
        project/includes/core
        project/includes/core/reader
        project/includes/core/reader/cfgReader
        project/includes/Manager
        project/includes/math
        project/includes/render
        project/includes/Manager/Default
        project/includes/Manager/client
        project/includes/Manager/server
        project/includes/security
        project/includes/Error
        ${LIBCONFIG_INCLUDE_DIRS}
        shared/Light
        shared/Object
        shared/Texture
)

target_compile_options(raytracer
    PRIVATE
        -Wall
        -Wextra
        -Werror
        -lconfig++
)

target_link_libraries(raytracer
    PRIVATE
        ${LIBCONFIG_LIBRARIES}
)
