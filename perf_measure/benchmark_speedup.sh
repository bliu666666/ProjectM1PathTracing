#!/bin/bash

# Script de benchmark pour comparer les performances CPU vs GPU en fonction du nombre de threads
# Auteur: Cascade
# Date: $(date +"%Y-%m-%d")

# Configuration
OUTPUT_CSV="speedup_results.csv"
BUILD_DIR="../build"
EXECUTABLE="render"
SCENE="test"  # Nom de la scène à utiliser
WIDTH=1024
HEIGHT=1024
SAMPLES=16
MAX_DEPTH=5
OUTPUT_PREFIX="output"

# Assurez-vous que le répertoire build existe
mkdir -p $BUILD_DIR

# Créer le fichier CSV avec les en-têtes
echo "Threads,CPU_Time,GPU_Time,Speedup,Resolution,Samples" > $OUTPUT_CSV

# Fonction pour exécuter un benchmark
run_benchmark() {
    local threads=$1
    local use_gpu=$2
    local width=$3
    local height=$4
    local samples=$5
    local output_file="${OUTPUT_PREFIX}_${use_gpu}_${threads}_${width}x${height}_${samples}.ppm"
    
    # Définir le nombre de threads OpenMP
    export OMP_NUM_THREADS=$threads
    
    # Construire la commande
    local cmd=""
    if [ "$use_gpu" = "gpu" ]; then
        # Vérifier si le build avec CUDA existe
        if [ ! -f "$BUILD_DIR/$EXECUTABLE" ]; then
            echo "Erreur: L'exécutable GPU n'existe pas. Compilez d'abord avec CUDA."
            exit 1
        fi
        cmd="$BUILD_DIR/$EXECUTABLE --gpu --scene $SCENE --width $width --height $height --samples $samples --depth $MAX_DEPTH --output $output_file"
    else
        # Vérifier si le build CPU existe
        if [ ! -f "$BUILD_DIR/$EXECUTABLE" ]; then
            echo "Erreur: L'exécutable CPU n'existe pas. Compilez d'abord."
            exit 1
        fi
        cmd="$BUILD_DIR/$EXECUTABLE --scene $SCENE --width $width --height $height --samples $samples --depth $MAX_DEPTH --output $output_file"
    fi
    
    # Exécuter et mesurer le temps
    echo "Exécution avec $threads threads, mode: $use_gpu, résolution: ${width}x${height}, échantillons: $samples"
    start_time=$(date +%s.%N)
    $cmd
    end_time=$(date +%s.%N)
    execution_time=$(echo "$end_time - $start_time" | bc)
    
    echo "Temps d'exécution: $execution_time secondes"
    echo "$execution_time"
}

# Tester différentes configurations de threads pour CPU
for threads in 1 2 4 8 16 32 64 128; do
    # Exécuter le benchmark CPU
    cpu_time=$(run_benchmark $threads "cpu" $WIDTH $HEIGHT $SAMPLES)
    
    # Exécuter le benchmark GPU (le nombre de threads CPU n'affecte pas directement le GPU,
    # mais peut affecter les parties CPU du code GPU)
    gpu_time=$(run_benchmark $threads "gpu" $WIDTH $HEIGHT $SAMPLES)
    
    # Calculer le speedup
    speedup=$(echo "scale=2; $cpu_time / $gpu_time" | bc)
    
    # Enregistrer les résultats
    echo "$threads,$cpu_time,$gpu_time,$speedup,$WIDTH x $HEIGHT,$SAMPLES" >> $OUTPUT_CSV
    
    echo "Speedup avec $threads threads: $speedup"
done

# Générer un graphique avec gnuplot si disponible
if command -v gnuplot &> /dev/null; then
    echo "Génération du graphique de speedup..."
    gnuplot <<- EOF
    set terminal png size 800,800
    set output "speedup_graph.png"
    set title "Speedup GPU vs CPU en fonction du nombre de threads"
    set xlabel "Nombre de threads CPU"
    set ylabel "Speedup (CPU time / GPU time)"
    set grid
    set key top left
    plot "$OUTPUT_CSV" using 1:4 with linespoints lw 2 title "Speedup"
EOF
    echo "Graphique généré: speedup_graph.png"
else
    echo "gnuplot n'est pas installé. Le graphique n'a pas été généré."
fi

echo "Benchmark terminé. Résultats enregistrés dans $OUTPUT_CSV"

