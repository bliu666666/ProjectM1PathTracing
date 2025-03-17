cmake_minimum_required(VERSION 3.22)

project(ProjectM1PathTracing)

# Option pour activer le profilage avec gprof
option(ENABLE_PROFILING "Enable profiling with gprof" OFF)

# Option pour activer le mode automatique (sans interaction utilisateur)
option(AUTO_CAMERA_PARAMS "Use predefined camera parameters" OFF)

set(CMAKE_CXX_STANDARD 17)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

set(PPM_WRITER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/ppm_writer")

file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp" "${PPM_WRITER_DIR}/ppm_writer.c")

add_executable(render ${SOURCES})

# Ajouter les flags de compilation pour gprof si l'option est activée
if(ENABLE_PROFILING)
    set_target_properties(render PROPERTIES COMPILE_FLAGS "-pg")
    set_target_properties(render PROPERTIES LINK_FLAGS "-pg")
endif()

# Ajouter la définition pour le mode automatique si l'option est activée
if(AUTO_CAMERA_PARAMS)
    target_compile_definitions(render PRIVATE AUTO_CAMERA_PARAMS)
endif()

# Si les deux options sont activées, configurer pour le profiling automatique
if(ENABLE_PROFILING AND AUTO_CAMERA_PARAMS)
    
    # Créer un fichier de script pour exécuter gprof
    file(WRITE ${CMAKE_BINARY_DIR}/run_gprof.sh "
#!/bin/bash

# Exécuter le programme pour générer le fichier gmon.out
./render \$@

# Exécuter gprof pour générer le rapport de profilage
gprof render gmon.out > profile_report.txt

echo \"Rapport de profilage généré dans profile_report.txt\"
")
    
    # Rendre le script exécutable
    execute_process(COMMAND chmod +x ${CMAKE_BINARY_DIR}/run_gprof.sh)
    
    # Ajouter une cible personnalisée pour exécuter gprof
    add_custom_target(profile
        COMMAND ${CMAKE_BINARY_DIR}/run_gprof.sh ${ARGS}
        DEPENDS render
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Exécution du programme avec profilage gprof")
endif()

target_link_libraries(render m)

target_include_directories(render PUBLIC "${SRC_DIR}" "${PPM_WRITER_DIR}")