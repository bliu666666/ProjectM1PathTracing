#!/bin/bash


echo "======================================================================"
echo "                      PROFILING AVEC GPROF                            "
echo "======================================================================"

# Créer un Dockerfile minimal
cat > Dockerfile.profiling << 'EOF'
FROM --platform=linux/amd64 ubuntu:22.04

# Éviter les interactions
ENV DEBIAN_FRONTEND=noninteractive

# Installer les outils nécessaires
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    gdb \
    && rm -rf /var/lib/apt/lists/*

# Répertoire de travail
WORKDIR /app
EOF

# Construire l'image Docker
docker build -f Dockerfile.profiling -t pathtracer-profiling --platform linux/amd64 .

# Créer un répertoire pour les résultats
mkdir -p profiling_results

# Exécuter le conteneur avec les volumes montés
# Pour Windows, utilisez %cd% au lieu de $(pwd) si vous exécutez via cmd.exe
# Pour PowerShell sur Windows, utilisez ${PWD} au lieu de $(pwd)
# La syntaxe ci-dessous devrait fonctionner sur Linux et macOS
docker run --rm --platform linux/amd64 \
    -v "$(pwd):/app" \
    pathtracer-profiling /bin/bash -c '
    cd /app
    rm -rf build
    mkdir -p build
    cd build
    
    cmake .. -DENABLE_PROFILING=ON -DAUTO_CAMERA_PARAMS=ON
    make -j$(nproc)
    
    if [ -f "./render" ]; then
        ./render 1080 720 100 output.ppm 10
        
        if [ -f "gmon.out" ]; then
            echo "Génération du rapport gprof..."
            gprof ./render gmon.out > profile_report.txt
            
            echo "========= TOP 10 FONCTIONS LES PLUS COÛTEUSES ========="
            grep -A 15 "flat  flat" profile_report.txt | tail -n +3 | head -n 10
            echo "====================================================="
            
            # Créer un répertoire pour les résultats dans le volume monté
            mkdir -p /app/profiling_results
            cp profile_report.txt /app/profiling_results/
            cp gmon.out /app/profiling_results/
            cp output.ppm /app/profiling_results/ 2>/dev/null || true
            
            echo "Rapport sauvegardé dans /app/profiling_results/"
            
            # Afficher le contenu du répertoire pour vérification
            ls -la /app/profiling_results/
        else
            echo "ERREUR: Le fichier gmon.out na pas été généré."
            exit 1
        fi
    else
        echo "ERREUR: Lexécutable render na pas été créé."
        exit 1
    fi
'

# Nettoyage
rm Dockerfile.profiling

echo "======================================================================"
echo "Profiling terminé! Les résultats sont disponibles dans ./profiling_results/"
echo "Pour analyser les résultats: cat ./profiling_results/profile_report.txt"
echo "======================================================================"
