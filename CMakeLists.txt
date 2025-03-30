cmake_minimum_required(VERSION 3.22)

project(ProjectM1PathTracing)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP REQUIRED)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(PPM_WRITER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ppm_writer")

file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp" "${PPM_WRITER_DIR}/ppm_writer.c")

add_executable(render ${SOURCES})

target_link_libraries(render m OpenMP::OpenMP_CXX)

target_include_directories(render PUBLIC "${SRC_DIR}" "${PPM_WRITER_DIR}")