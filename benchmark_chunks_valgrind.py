#!/usr/bin/env python3

import argparse
import subprocess
import os
import time
import csv
import matplotlib.pyplot as plt
from pathlib import Path

def run_valgrind_benchmark(width, height, samples, depth, mode, threads, chunk_sizes, output_dir):
    """Exécute le benchmark avec Valgrind DRD pour chaque taille de chunk."""
    results = []
    
    # Créer les dossiers de sortie
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    valgrind_dir = os.path.join(output_dir, 'valgrind_logs')
    if not os.path.exists(valgrind_dir):
        os.makedirs(valgrind_dir)
    
    # Fichier CSV pour stocker les résultats
    csv_file = os.path.join(output_dir, 'valgrind_benchmark_results.csv')
    
    # En-tête du CSV
    with open(csv_file, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(['Chunk Size', 'Time (s)', 'Data Race Conflicts', 'False Sharing Detected'])
    
    for chunk_size in chunk_sizes:
        print(f"\nTest avec {threads} threads et chunk_size = {chunk_size}")
        
        # Nom du fichier de sortie pour ce test
        output_file = os.path.join(output_dir, f'output_chunk_{chunk_size}.ppm')
        log_file = os.path.join(valgrind_dir, f'drd_chunk_{chunk_size}.log')
        
        # Construire la commande Valgrind
        cmd = ['valgrind',
               '--tool=drd',
               '--check-stack-var=yes',
               '--report-signal-unlocked=yes',
               '--segment-merging=no',
               '--shared-threshold=10',
               '--show-confl-seg=yes',
               f'--log-file={log_file}',
               './build/render',
               str(width),
               str(height),
               str(samples),
               output_file,
               str(depth),
               str(mode),
               str(threads),
               str(chunk_size)]
        
        # Mesurer le temps d'exécution
        start_time = time.time()
        process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, stderr = process.communicate()
        execution_time = time.time() - start_time
        
        # Analyser le fichier log de Valgrind
        data_races = 0
        false_sharing = 0
        
        if os.path.exists(log_file):
            with open(log_file, 'r') as f:
                log_content = f.read()
                # Compter les conflits de data races
                data_races = log_content.count('Conflicting access')
                # Détecter le false sharing (segments partagés avec conflits)
                false_sharing = log_content.count('Concurrent segment access')
        
        # Sauvegarder les résultats
        results.append((chunk_size, execution_time, data_races, false_sharing))
        
        # Écrire dans le CSV
        with open(csv_file, 'a', newline='') as f:
            writer = csv.writer(f)
            writer.writerow([chunk_size, execution_time, data_races, false_sharing])
        
        print(f"Temps d'exécution: {execution_time:.2f} secondes")
        print(f"Data races détectées: {data_races}")
        print(f"Potentiel false sharing: {false_sharing}")
        
        # Supprimer le fichier de sortie pour économiser de l'espace
        if os.path.exists(output_file):
            os.remove(output_file)
    
    return results

def plot_results(results, output_dir):
    """Génère des graphiques à partir des résultats."""
    chunk_sizes = [r[0] for r in results]
    times = [r[1] for r in results]
    data_races = [r[2] for r in results]
    false_sharing = [r[3] for r in results]
    
    # Graphique du temps d'exécution
    plt.figure(figsize=(10, 6))
    plt.plot(chunk_sizes, times, 'o-', label='Temps d\'exécution')
    plt.xlabel('Taille des chunks')
    plt.ylabel('Temps (secondes)')
    plt.title('Temps d\'exécution en fonction de la taille des chunks')
    plt.xticks(chunk_sizes)
    plt.grid(True)
    plt.legend()
    plt.savefig(os.path.join(output_dir, 'execution_time_valgrind.png'))
    plt.close()
    
    # Graphique des problèmes de concurrence
    plt.figure(figsize=(10, 6))
    plt.plot(chunk_sizes, data_races, 'r-o', label='Data Races')
    plt.plot(chunk_sizes, false_sharing, 'b-o', label='False Sharing')
    plt.xlabel('Taille des chunks')
    plt.ylabel('Nombre de conflits')
    plt.title('Problèmes de concurrence en fonction de la taille des chunks')
    plt.xticks(chunk_sizes)
    plt.grid(True)
    plt.legend()
    plt.savefig(os.path.join(output_dir, 'concurrency_issues.png'))
    plt.close()

def main():
    parser = argparse.ArgumentParser(description='Benchmark des chunks avec analyse Valgrind')
    
    # Arguments pour la configuration du benchmark
    parser.add_argument('--width', type=int, default=200, help='Largeur de l\'image (défaut: 200)')
    parser.add_argument('--height', type=int, default=200, help='Hauteur de l\'image (défaut: 200)')
    parser.add_argument('--samples', type=int, default=50, help='Nombre d\'échantillons par pixel (défaut: 50)')
    parser.add_argument('--depth', type=int, default=5, help='Profondeur maximale des rebonds (défaut: 5)')
    parser.add_argument('--mode', type=int, default=0, help='Mode de rendu (0: normal, 1: MLT, défaut: 0)')
    parser.add_argument('--threads', type=int, default=16, help='Nombre de threads à utiliser (défaut: 16)')
    parser.add_argument('--output-dir', type=str, default='benchmark_valgrind_results', 
                       help='Dossier de sortie pour les résultats (défaut: benchmark_valgrind_results)')
    
    args = parser.parse_args()
    
    # Tailles de chunks à tester (réduites pour l'analyse Valgrind)
    chunk_sizes = [1, 2, 4, 8, 16]
    
    print("Démarrage du benchmark avec analyse Valgrind...")
    print(f"Configuration:")
    print(f"- Résolution: {args.width}x{args.height}")
    print(f"- Échantillons par pixel: {args.samples}")
    print(f"- Profondeur maximale: {args.depth}")
    print(f"- Mode de rendu: {'Normal' if args.mode == 0 else 'MLT'}")
    print(f"- Nombre de threads: {args.threads}")
    print(f"- Tailles de chunks à tester: {chunk_sizes}")
    print("\nAttention: Cette analyse peut prendre beaucoup de temps!")
    
    # Exécuter le benchmark
    results = run_valgrind_benchmark(
        args.width, args.height,
        args.samples, args.depth,
        args.mode, args.threads,
        chunk_sizes, args.output_dir
    )
    
    # Générer les graphiques
    plot_results(results, args.output_dir)
    
    print(f"\nBenchmark terminé. Résultats sauvegardés dans {args.output_dir}/")
    print("Consultez les fichiers logs dans le dossier valgrind_logs/ pour plus de détails.")

if __name__ == '__main__':
    main() 