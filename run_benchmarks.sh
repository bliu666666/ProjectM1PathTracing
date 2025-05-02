#!/bin/bash

# Couleurs pour les messages
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}=== Démarrage de la suite de benchmarks ===${NC}"

# 1. Weak Scaling (dimension)
echo -e "\n${GREEN}=== Benchmark Weak Scaling (dimension) ===${NC}"
python3 benchmark_weak_scaling.py \
    --base-width 200 \
    --base-height 200 \
    --samples 50 \
    --depth 5 \
    --max-threads 128 \
    --output-dir benchmarks_weak_scaling

# 2. Weak Scaling (samples)
echo -e "\n${GREEN}=== Benchmark Weak Scaling (samples) ===${NC}"
python3 benchmark_weak_scaling_samples.py \
    --width 200 \
    --height 200 \
    --base-samples 50 \
    --depth 5 \
    --max-threads 128 \
    --output-dir benchmarks_weak_scaling_samples

# 3. Strong Scaling
echo -e "\n${GREEN}=== Benchmark Strong Scaling ===${NC}"
python3 benchmark_strong_scaling.py \
    --width 800 \
    --height 600 \
    --samples 500 \
    --depth 5 \
    --max-threads 128 \
    --output-dir benchmarks_strong_scaling

# 4. Chunks
echo -e "\n${GREEN}=== Benchmark Chunks ===${NC}"
python3 benchmark_chunks.py \
    --width 400 \
    --height 300 \
    --samples 100 \
    --depth 5 \
    --mode 0 \
    --threads 16 \
    --output-dir benchmarks_chunks

# 5. Chunks avec Valgrind
echo -e "\n${GREEN}=== Benchmark Chunks avec Valgrind ===${NC}"
echo "Attention: Cette analyse peut prendre beaucoup de temps!"
python3 benchmark_chunks_valgrind.py \
    --width 200 \
    --height 200 \
    --samples 50 \
    --depth 5 \
    --mode 0 \
    --threads 16 \
    --output-dir benchmarks_chunks_valgrind

echo -e "\n${BLUE}=== Tous les benchmarks sont terminés ===${NC}"
echo "Les résultats sont disponibles dans les dossiers:"
echo "- benchmarks_weak_scaling/"
echo "- benchmarks_weak_scaling_samples/"
echo "- benchmarks_strong_scaling/"
echo "- benchmarks_chunks/"
echo "- benchmarks_chunks_valgrind/" 