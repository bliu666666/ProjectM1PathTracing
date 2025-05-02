#!/bin/bash

# Fonction pour afficher l'aide
show_help() {
    echo "Usage: $0 [options]"
    echo "Options:"
    echo "  -m, --mode MODE       Mode d'analyse:"
    echo "                        - leak: détection des fuites mémoire"
    echo "                        - cache: analyse du cache"
    echo "                        - call: analyse des appels"
    echo "                        - drd: détection des data races et false sharing"
    echo "                        - helgrind: détection des problèmes de synchronisation"
    echo "                        - all: toutes les analyses"
    echo "  -w, --width WIDTH     Largeur de l'image (défaut: 800)"
    echo "  -h, --height HEIGHT   Hauteur de l'image (défaut: 600)"
    echo "  -s, --samples SAMPLES Nombre d'échantillons par pixel (défaut: 100)"
    echo "  -d, --depth DEPTH     Profondeur maximale (défaut: 10)"
    echo "  -c, --chunk CHUNK     Taille des chunks (défaut: 1)"
    echo "  -o, --output OUTPUT   Fichier de sortie (défaut: output.ppm)"
    echo "  -q, --quick           Mode rapide avec paramètres optimisés"
    echo "  --help                Afficher cette aide"
}

# Valeurs par défaut
MODE="leak"
WIDTH=800
HEIGHT=600
SAMPLES=100
DEPTH=10
CHUNK=1
OUTPUT="output.ppm"
QUICK=false

# Parser les arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -m|--mode)
            MODE="$2"
            shift 2
            ;;
        -w|--width)
            WIDTH="$2"
            shift 2
            ;;
        -h|--height)
            HEIGHT="$2"
            shift 2
            ;;
        -s|--samples)
            SAMPLES="$2"
            shift 2
            ;;
        -d|--depth)
            DEPTH="$2"
            shift 2
            ;;
        -c|--chunk)
            CHUNK="$2"
            shift 2
            ;;
        -o|--output)
            OUTPUT="$2"
            shift 2
            ;;
        -q|--quick)
            QUICK=true
            WIDTH=200
            HEIGHT=200
            SAMPLES=50
            DEPTH=5
            shift
            ;;
        --help)
            show_help
            exit 0
            ;;
        *)
            echo "Option inconnue: $1"
            show_help
            exit 1
            ;;
    esac
done

# Créer le dossier de sortie s'il n'existe pas
mkdir -p valgrind_output

# Fonction pour exécuter l'analyse des fuites de mémoire
run_leak_check() {
    echo "Analyse des fuites de mémoire..."
    valgrind --leak-check=full \
            --show-leak-kinds=all \
            --errors-for-leak-kinds=all \
            --log-file="valgrind_output/leak_check_$(date +%Y%m%d_%H%M%S).log" \
            ./build/render $WIDTH $HEIGHT $SAMPLES $OUTPUT $DEPTH 0 $CHUNK
}

# Fonction pour exécuter l'analyse de cache
run_cache_analysis() {
    echo "Analyse des performances (cache)..."
    valgrind --tool=cachegrind \
            --log-file="valgrind_output/cache_analysis_$(date +%Y%m%d_%H%M%S).log" \
            ./build/render $WIDTH $HEIGHT $SAMPLES $OUTPUT $DEPTH 0 $CHUNK
}

# Fonction pour exécuter l'analyse des appels
run_call_analysis() {
    echo "Analyse des performances (callgrind)..."
    valgrind --tool=callgrind \
            --log-file="valgrind_output/call_analysis_$(date +%Y%m%d_%H%M%S).log" \
            ./build/render $WIDTH $HEIGHT $SAMPLES $OUTPUT $DEPTH 0 $CHUNK
}

# Fonction pour détecter les data races et false sharing avec DRD
run_drd_analysis() {
    echo "Analyse des data races et false sharing avec DRD..."
    valgrind --tool=drd \
            --check-stack-var=yes \
            --report-signal-unlocked=yes \
            --segment-merging=no \
            --shared-threshold=10 \
            --show-confl-seg=yes \
            --log-file="valgrind_output/drd_analysis_$(date +%Y%m%d_%H%M%S).log" \
            ./build/render $WIDTH $HEIGHT $SAMPLES $OUTPUT $DEPTH 0 $CHUNK
}

# Fonction pour détecter les problèmes de synchronisation avec Helgrind
run_helgrind_analysis() {
    echo "Analyse des problèmes de synchronisation avec Helgrind..."
    valgrind --tool=helgrind \
            --history-level=full \
            --conflict-cache-size=10000000 \
            --log-file="valgrind_output/helgrind_analysis_$(date +%Y%m%d_%H%M%S).log" \
            ./build/render $WIDTH $HEIGHT $SAMPLES $OUTPUT $DEPTH 0 $CHUNK
}

# Exécuter l'analyse selon le mode choisi
case $MODE in
    "leak")
        run_leak_check
        ;;
    "cache")
        run_cache_analysis
        ;;
    "call")
        run_call_analysis
        ;;
    "drd")
        run_drd_analysis
        ;;
    "helgrind")
        run_helgrind_analysis
        ;;
    "all")
        run_leak_check
        run_cache_analysis
        run_call_analysis
        run_drd_analysis
        run_helgrind_analysis
        ;;
    *)
        echo "Mode inconnu: $MODE"
        show_help
        exit 1
        ;;
esac

echo "Analyse terminée. Les résultats sont dans le dossier valgrind_output/"

# Afficher un résumé des résultats pour DRD et Helgrind
if [[ $MODE == "drd" || $MODE == "all" ]]; then
    echo -e "\nRésumé de l'analyse DRD (data races et false sharing):"
    grep -A 5 "ERROR SUMMARY" "valgrind_output/drd_analysis_"*".log" | tail -n 6
fi

if [[ $MODE == "helgrind" || $MODE == "all" ]]; then
    echo -e "\nRésumé de l'analyse Helgrind (synchronisation):"
    grep -A 5 "ERROR SUMMARY" "valgrind_output/helgrind_analysis_"*".log" | tail -n 6
fi 