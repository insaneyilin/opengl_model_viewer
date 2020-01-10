cmake_minimum_required(VERSION 2.8)
project(opengl_viewer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED STATIC)
find_package(Eigen3)
include_directories(${EIGEN3_INCLUDE_DIR})

add_executable(opengl_viewer main.cc gl_app.cc glsl_shader.cc coordinate_axes.cc)
target_link_libraries(opengl_viewer
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    glfw)
