# Outils de mesure de performance pour Path Tracer CPU/GPU

Ce répertoire contient des scripts pour mesurer et analyser les performances comparatives des implémentations CPU et GPU du path tracer.

## Scripts disponibles

### 1. `benchmark_speedup.sh`

Script shell qui mesure les temps d'exécution CPU et GPU en fonction du nombre de threads OpenMP, et calcule le speedup.

**Fonctionnalités :**
- Teste différentes configurations de threads (1, 2, 4, 8, 16)
- Génère un fichier CSV avec les résultats
- Crée un graphique simple si gnuplot est installé

**Utilisation :**
```bash
# Rendre le script exécutable
chmod +x benchmark_speedup.sh

# Exécuter le benchmark
./benchmark_speedup.sh
```

### 2. `generate_plots.py`

Script Python qui génère des graphiques détaillés à partir des résultats du benchmark.

**Graphiques générés :**
- Speedup GPU vs CPU en fonction du nombre de threads
- Temps d'exécution comparatifs CPU vs GPU
- Efficacité parallèle CPU
- Performance relative en rayons par seconde

**Prérequis :**
- Python 3
- Pandas
- Matplotlib

**Utilisation :**
```bash
# Installer les dépendances
pip install pandas matplotlib

# Exécuter le script
python3 generate_plots.py
```

## Compilation
1. Compiler le projet avec et sans CUDA :
   ```bash
   # Compilation CPU
   mkdir -p build_cpu
   cd build_cpu
   cmake ..
   make -j

   # Compilation GPU
   mkdir -p build_gpu
   cd build_gpu
   cmake -DUSE_CUDA=ON ..
   make -j
   ```

2. Exécuter le benchmark :
   ```bash
   cd perf_measure
   ./benchmark_speedup.sh
   ```

3. Générer les graphiques :
   ```bash
   python3 generate_plots.py
   ```

4. Analyser les résultats dans le dossier `plots/`

