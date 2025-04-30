@echo off
REM Script de profiling pour Windows

echo ======================================================================
echo                      PROFILING AVEC GPROF                            
echo ======================================================================

REM Créer un Dockerfile minimal
echo FROM --platform=linux/amd64 ubuntu:22.04 > Dockerfile.profiling
echo. >> Dockerfile.profiling
echo ENV DEBIAN_FRONTEND=noninteractive >> Dockerfile.profiling
echo. >> Dockerfile.profiling
echo RUN apt-get update ^&^& apt-get install -y ^\ >> Dockerfile.profiling
echo     build-essential ^\ >> Dockerfile.profiling
echo     cmake ^\ >> Dockerfile.profiling
echo     gdb ^\ >> Dockerfile.profiling
echo     ^&^& rm -rf /var/lib/apt/lists/* >> Dockerfile.profiling
echo. >> Dockerfile.profiling
echo WORKDIR /app >> Dockerfile.profiling

REM Construire l'image Docker
docker build -f Dockerfile.profiling -t pathtracer-profiling --platform linux/amd64 .

REM Créer un répertoire pour les résultats
if not exist profiling_results mkdir profiling_results

REM Exécuter le conteneur avec les volumes montés
docker run --rm --platform linux/amd64 -v "%cd%:/app" pathtracer-profiling /bin/bash -c "cd /app && rm -rf build && mkdir -p build && cd build && cmake .. -DENABLE_PROFILING=ON -DAUTO_CAMERA_PARAMS=ON && make -j$(nproc) && if [ -f ./render ]; then ./render 800 600 50 output.ppm 10 && if [ -f gmon.out ]; then echo 'Génération du rapport gprof...' && gprof ./render gmon.out > profile_report.txt && echo '========= TOP 10 FONCTIONS LES PLUS COÛTEUSES =========' && grep -A 15 'flat  flat' profile_report.txt | tail -n +3 | head -n 10 && echo '=====================================================' && mkdir -p /app/profiling_results && cp profile_report.txt /app/profiling_results/ && cp gmon.out /app/profiling_results/ && cp output.ppm /app/profiling_results/ 2>/dev/null || true && echo 'Rapport sauvegardé dans /app/profiling_results/' && ls -la /app/profiling_results/; else echo 'ERREUR: Le fichier gmon.out na pas été généré.' && exit 1; fi; else echo 'ERREUR: Lexécutable render na pas été créé.' && exit 1; fi"

REM Nettoyage
del Dockerfile.profiling

echo ======================================================================
echo Profiling terminé! Les résultats sont disponibles dans ./profiling_results/
echo Pour analyser les résultats: type .\profiling_results\profile_report.txt
echo ======================================================================
