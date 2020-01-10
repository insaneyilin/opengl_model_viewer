cmake_minimum_required(VERSION 2.8)
project(opengl_viewer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(${GLFW_LIBRARY_DIRS})

find_package(GLEW REQUIRED STATIC)
include_directories(${GLEW_INCLUDE_DIRS})

find_package(Eigen3)
include_directories(${EIGEN3_INCLUDE_DIR})

if (APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OpenGL_LIBRARY OpenGL)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    set(EXTRA_LIBS ${COCOA_LIBRARY} ${OpenGL_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
endif (APPLE)

file(GLOB _SRCS ./*.cc)

add_executable(opengl_viewer ${_SRCS})
target_link_libraries(opengl_viewer
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARY}
    glfw
    GLEW
    ${EXTRA_LIBS})
