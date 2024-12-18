cmake_minimum_required(VERSION 3.10)

project(RayTracer)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

message(STATUS "Configuration du projet RayTracer")
message(STATUS "Fichiers sources : ${SOURCES}")


# Options de compilation
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Chemins des dossiers sources
set(SRC_DIR src)
set(SHAPES_DIR shapes)
set(PPM_WRITER_DIR ppm_writer)

# Ajouter tous les fichiers source
set(SOURCES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/camera.cpp
    ${SRC_DIR}/entity.cpp
    ${SRC_DIR}/position.cpp
    ${SRC_DIR}/ray.cpp
    ${SRC_DIR}/renderer.cpp
    ${SRC_DIR}/scene.cpp
    ${SHAPES_DIR}/object.cpp
    ${SHAPES_DIR}/sphere.cpp
    ${SHAPES_DIR}/cube.cpp
    ${PPM_WRITER_DIR}/ppm_writer.c
)

# Inclure les headers
include_directories(
    ${SRC_DIR}
    ${SHAPES_DIR}
    ${PPM_WRITER_DIR}
)

# Ajouter l'exécutable
add_executable(${PROJECT_NAME} ${SOURCES})
