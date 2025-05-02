#!/usr/bin/env python3

import argparse
import subprocess
import os
import time
import csv
import matplotlib.pyplot as plt
from pathlib import Path

def run_benchmark(width, height, samples, depth, mode, threads, chunk_sizes, output_dir):
    """Exécute le benchmark avec différentes tailles de chunks."""
    results = []
    
    # Créer le dossier de sortie s'il n'existe pas
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    
    # Fichier CSV pour stocker les résultats
    csv_file = os.path.join(output_dir, 'chunk_benchmark_results.csv')
    
    # En-tête du CSV
    with open(csv_file, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(['Chunk Size', 'Time (s)', 'Speedup'])
    
    # Temps de référence (chunk size = 1)
    reference_time = None
    
    for chunk_size in chunk_sizes:
        print(f"\nTest avec {threads} threads et chunk_size = {chunk_size}")
        
        # Nom du fichier de sortie pour ce test
        output_file = os.path.join(output_dir, f'output_chunk_{chunk_size}.ppm')
        
        # Construire la commande
        cmd = ['./build/render', 
               str(width), 
               str(height), 
               str(samples), 
               output_file, 
               str(depth), 
               str(mode),
               str(threads),  # nombre de threads
               str(chunk_size)]
        
        # Mesurer le temps d'exécution
        start_time = time.time()
        process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, stderr = process.communicate()
        end_time = time.time()
        
        execution_time = end_time - start_time
        
        # Calculer le speedup par rapport au temps de référence
        if reference_time is None:
            reference_time = execution_time
            speedup = 1.0
        else:
            speedup = reference_time / execution_time
        
        # Sauvegarder les résultats
        results.append((chunk_size, execution_time, speedup))
        
        # Écrire dans le CSV
        with open(csv_file, 'a', newline='') as f:
            writer = csv.writer(f)
            writer.writerow([chunk_size, execution_time, speedup])
        
        print(f"Temps d'exécution: {execution_time:.2f} secondes")
        print(f"Speedup: {speedup:.2f}x")
        
        # Supprimer le fichier de sortie pour économiser de l'espace
        if os.path.exists(output_file):
            os.remove(output_file)
    
    return results

def plot_results(results, output_dir, threads):
    """Génère des graphiques à partir des résultats."""
    chunk_sizes = [r[0] for r in results]
    times = [r[1] for r in results]
    speedups = [r[2] for r in results]
    
    # Graphique du temps d'exécution
    plt.figure(figsize=(10, 6))
    plt.plot(chunk_sizes, times, 'o-', label='Temps d\'exécution')
    plt.xlabel('Taille des chunks')
    plt.ylabel('Temps (secondes)')
    plt.title(f'Temps d\'exécution en fonction de la taille des chunks\n({threads} threads)')
    plt.xticks(chunk_sizes)  # Force l'affichage des valeurs exactes des chunks
    plt.grid(True)
    plt.savefig(os.path.join(output_dir, 'execution_time.png'))
    plt.close()
    
    # Graphique du speedup
    plt.figure(figsize=(10, 6))
    plt.plot(chunk_sizes, speedups, 'o-', label='Speedup')
    plt.axhline(y=1, color='r', linestyle='--', label='Référence (chunk=1)')
    plt.xlabel('Taille des chunks')
    plt.ylabel('Speedup')
    plt.title(f'Speedup en fonction de la taille des chunks\n({threads} threads)')
    plt.xticks(chunk_sizes)  # Force l'affichage des valeurs exactes des chunks
    plt.grid(True)
    plt.legend()
    plt.savefig(os.path.join(output_dir, 'speedup.png'))
    plt.close()

def main():
    parser = argparse.ArgumentParser(description='Benchmark des différentes tailles de chunks')
    
    # Arguments pour la configuration du benchmark
    parser.add_argument('--width', type=int, default=400, help='Largeur de l\'image (défaut: 400)')
    parser.add_argument('--height', type=int, default=300, help='Hauteur de l\'image (défaut: 300)')
    parser.add_argument('--samples', type=int, default=50, help='Nombre d\'échantillons par pixel (défaut: 50)')
    parser.add_argument('--depth', type=int, default=10, help='Profondeur maximale des rebonds (défaut: 10)')
    parser.add_argument('--mode', type=int, default=0, help='Mode de rendu (0: normal, 1: MLT, défaut: 0)')
    parser.add_argument('--threads', type=int, default=16, help='Nombre de threads à utiliser (défaut: 16)')
    parser.add_argument('--output-dir', type=str, default='benchmark_results', help='Dossier de sortie pour les résultats (défaut: benchmark_results)')
    
    args = parser.parse_args()
    
    # Tailles de chunks à tester
    chunk_sizes = [1, 2, 4, 8, 16, 32, 64]
    
    print("Démarrage du benchmark des chunks...")
    print(f"Configuration:")
    print(f"- Résolution: {args.width}x{args.height}")
    print(f"- Échantillons par pixel: {args.samples}")
    print(f"- Profondeur maximale: {args.depth}")
    print(f"- Mode de rendu: {'Normal' if args.mode == 0 else 'MLT'}")
    print(f"- Nombre de threads: {args.threads}")
    print(f"- Tailles de chunks à tester: {chunk_sizes}")
    
    # Exécuter le benchmark
    results = run_benchmark(
        args.width, args.height,
        args.samples, args.depth,
        args.mode, args.threads,
        chunk_sizes, args.output_dir
    )
    
    # Générer les graphiques
    plot_results(results, args.output_dir, args.threads)
    
    print(f"\nBenchmark terminé. Résultats sauvegardés dans {args.output_dir}/")

if __name__ == '__main__':
    main()