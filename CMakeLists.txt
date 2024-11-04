cmake_minimum_required(VERSION 3.22)

project(ProjectM1PathTracing)

set(CMAKE_CXX_STANDARD 17)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")

add_executable(ProjectM1PathTracing ${SOURCES})

target_link_libraries(ProjectM1PathTracing m)

target_include_directories(ProjectM1PathTracing PUBLIC "${SRC_DIR}")