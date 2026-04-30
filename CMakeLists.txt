cmake_minimum_required(VERSION 3.10)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins)

project(raytracer CXX)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBCONFIG REQUIRED libconfig++)

add_executable(raytracer
    project/src/main.cpp
    project/src/core/Core.cpp
    project/src/core/Builder.cpp
    project/src/security/args.cpp
    project/src/Manager/client/clientManager.cpp
    project/src/Manager/server/Poll.cpp
    project/src/Manager/server/ServerManager.cpp
    project/src/math/Vector3D.cpp
    project/src/math/Point3D.cpp
    project/src/core/Ray.cpp
    project/src/render/Rectangle3D.cpp
    project/src/render/Camera.cpp
)

target_include_directories(raytracer
    PUBLIC
        project/includes
        project/includes/core
        project/includes/Manager
        project/includes/math
        project/includes/render
        project/includes/Manager/Default
        project/includes/Manager/client
        project/includes/Manager/server
        project/includes/security
        project/includes/Error
        ${LIBCONFIG_INCLUDE_DIRS}
)

target_compile_options(raytracer
    PRIVATE
        -Wall
        -Wextra
        -Werror
)

target_link_libraries(raytracer
    PRIVATE
    ${LIBCONFIG_LIBRARIES}
)
