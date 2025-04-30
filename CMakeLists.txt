cmake_minimum_required(VERSION 3.22)

project(ProjectM1PathTracing)

# Option pour activer le profilage avec gprof
option(ENABLE_PROFILING "Enable profiling with gprof" OFF)
# Option pour activer le mode automatique (sans interaction utilisateur)
option(AUTO_CAMERA_PARAMS "Use predefined camera parameters" OFF)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP REQUIRED)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(PPM_WRITER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ppm_writer")
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp" "${PPM_WRITER_DIR}/ppm_writer.cpp")

add_executable(render ${SOURCES})

# Activer le profilage avec gprof si demandé
if(ENABLE_PROFILING)
    set_target_properties(render PROPERTIES COMPILE_FLAGS "-pg" LINK_FLAGS "-pg")
endif()

# Activer le mode automatique si demandé
if(AUTO_CAMERA_PARAMS)
    target_compile_definitions(render PRIVATE AUTO_CAMERA_PARAMS)
endif()

# Linker avec math et OpenMP
target_link_libraries(render m OpenMP::OpenMP_CXX)

target_include_directories(render PUBLIC "${SRC_DIR}" "${PPM_WRITER_DIR}")