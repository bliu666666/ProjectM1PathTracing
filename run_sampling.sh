#!/bin/bash


WIDTH=400
HEIGHT=300
THREADS=8

# Dossier de sortie
OUTPUT_DIR="images"
mkdir -p "$OUTPUT_DIR"

# Boucle sur les échantillons
for SAMPLES in $(seq 100 100 1000); do
    OUTPUT_FILE="${OUTPUT_DIR}/rendu${SAMPLES}-${WIDTH}-${HEIGHT}.ppm"
    echo "Lancement : samples=$SAMPLES"
    ......................................................................................................


    
    python3 render_config.py --width $WIDTH --height $HEIGHT --samples $SAMPLES --threads $THREADS --output "$OUTPUT_FILE"
done
