FROM ubuntu:22.04

# Éviter les prompts interactifs
ENV DEBIAN_FRONTEND=noninteractive

# Installer les dépendances
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libomp-dev \
    gdb \
    binutils \
    && rm -rf /var/lib/apt/lists/*

# Créer un répertoire pour le projet
WORKDIR /app

# Copier les fichiers du projet
COPY . .

# Créer le dossier de build
RUN mkdir -p build

# Compiler le projet avec les options de profilage et de caméra automatique
RUN cd build && \
    cmake .. -DENABLE_PROFILING=ON -DAUTO_CAMERA_PARAMS=ON && \
    make -j$(nproc)

# Point d'entrée par défaut
CMD ["bash"]