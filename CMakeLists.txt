cmake_minimum_required(VERSION 3.10)

project(path_tracing)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

file(GLOB SOURCES ${SOURCE_DIR}/*.cpp)

include_directories(${SOURCE_DIR})

add_executable(path_tracing ${SOURCES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
