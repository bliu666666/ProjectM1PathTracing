cmake_minimum_required(VERSION 3.10)

project(RayTracer)

find_package(OpenMP REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

message(STATUS "Configuration du projet RayTracer")
message(STATUS "Fichiers sources : ${SOURCES}")

# Options de compilation
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_DIR src)
set(SHAPES_DIR shapes)
set(PPM_WRITER_DIR ppm_writer)


set(SOURCES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/camera.cpp
    ${SRC_DIR}/entity.cpp
    ${SRC_DIR}/position.cpp
    ${SRC_DIR}/ray.cpp
    ${SRC_DIR}/renderer.cpp
    ${SRC_DIR}/scene.cpp
    ${SRC_DIR}/Lambertian.cpp
    ${SRC_DIR}/Emissive.cpp
    ${SRC_DIR}/Metallic.cpp
    ${SRC_DIR}/Dielectric.cpp
    ${SHAPES_DIR}/object.cpp
    ${SHAPES_DIR}/sphere.cpp
    ${SHAPES_DIR}/AABB.cpp
    ${PPM_WRITER_DIR}/ppm_writer.c
)


include_directories(
    ${SRC_DIR}
    ${SHAPES_DIR}
    ${PPM_WRITER_DIR}
)


add_executable(${PROJECT_NAME} ${SOURCES})


target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX stdc++fs)
