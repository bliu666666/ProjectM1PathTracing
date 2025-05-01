cmake_minimum_required(VERSION 3.22)

# Option pour activer ou désactiver CUDA
option(USE_CUDA "Enable CUDA support" OFF)

project(ProjectM1PathTracing LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Configurer CUDA si activé
if(USE_CUDA)
    enable_language(CUDA)
    find_package(CUDA REQUIRED)
    set(CMAKE_CUDA_STANDARD 14)
    add_definitions(-DCUDA_ENABLED=1)
    message(STATUS "CUDA support enabled")
else()
    add_definitions(-DCUDA_ENABLED=0)
    message(STATUS "CUDA support disabled")
endif()

find_package(OpenMP REQUIRED)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(PPM_WRITER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ppm_writer")

# Collect source files
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp" "${PPM_WRITER_DIR}/ppm_writer.cpp")

# Add CUDA files if CUDA is enabled
if(USE_CUDA)
    file(GLOB_RECURSE CUDA_SOURCES "${SRC_DIR}/*.cu")
    list(APPEND SOURCES ${CUDA_SOURCES})
    
    # Set CUDA architectures
    set(CMAKE_CUDA_ARCHITECTURES 75 80 86)
    
    # Set CUDA specific flags
    set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS} -O3)
endif()

# Create the executable
add_executable(render ${SOURCES})

# Link libraries
if(USE_CUDA)
    target_link_libraries(render m OpenMP::OpenMP_CXX ${CUDA_LIBRARIES})
else()
    target_link_libraries(render m OpenMP::OpenMP_CXX)
endif()

# Include directories
target_include_directories(render PUBLIC "${SRC_DIR}" "${PPM_WRITER_DIR}")
if(USE_CUDA)
    target_include_directories(render PUBLIC ${CUDA_INCLUDE_DIRS})
endif()