#!/usr/bin/env python3

import argparse
import subprocess
import os
import time
import matplotlib.pyplot as plt
import numpy as np
from render_config import get_next_output_filename

def run_benchmark(width, height, samples, depth, threads, output):
    """Exécute le ray tracer avec les paramètres donnés et retourne le temps d'exécution."""
    cmd_args = ['./build/RayTracer']
    cmd_args.extend(['-w', str(width)])
    cmd_args.extend(['-h', str(height)])
    cmd_args.extend(['-s', str(samples)])
    cmd_args.extend(['-d', str(depth)])
    cmd_args.extend(['-t', str(threads)])
    cmd_args.extend(['-o', output])
    
    start_time = time.time()
    subprocess.run(cmd_args, check=True)
    end_time = time.time()
    
    return end_time - start_time

def main():
    parser = argparse.ArgumentParser(description='Mesurer le strong scaling du ray tracer')
    
    # Arguments pour la configuration du benchmark
    parser.add_argument('--width', type=int, default=800, help='Largeur de l\'image')
    parser.add_argument('--height', type=int, default=600, help='Hauteur de l\'image')
    parser.add_argument('--samples', type=int, default=100, help='Nombre d\'échantillons par pixel')
    parser.add_argument('--depth', type=int, default=50, help='Profondeur maximale des rebonds')
    parser.add_argument('--output-dir', type=str, default='benchmarks', help='Dossier pour sauvegarder les résultats')
    
    args = parser.parse_args()
    
    # Créer le dossier de sortie s'il n'existe pas
    if not os.path.exists(args.output_dir):
        os.makedirs(args.output_dir)
    
    # Nombre de threads fixe à tester
    thread_counts = [ 2, 4, 8, 16]
    
    # Mesures pour différents nombres de threads
    times = []
    speedups = []
    
    print("\nDébut des mesures de strong scaling...")
    for threads in thread_counts:
        print(f"\nTest avec {threads} threads...")
        output_file = get_next_output_filename(os.path.join(args.output_dir, f'threads_{threads}.ppm'))
        exec_time = run_benchmark(
            args.width, args.height, args.samples, args.depth,
            threads, output_file
        )
        times.append(exec_time)
        # Calcul du speedup par rapport au temps sur 1 thread
        speedup = times[0] / exec_time
        speedups.append(speedup)
        print(f"Temps d'exécution: {exec_time:.2f}s")
        print(f"Speedup: {speedup:.2f}x")
    
    # Sauvegarder les résultats
    results_file = os.path.join(args.output_dir, 'results.txt')
    with open(results_file, 'w') as f:
        f.write("Threads\tTime(s)\tSpeedup\n")
        for t, time, speedup in zip(thread_counts, times, speedups):
            f.write(f"{t}\t{time:.2f}\t{speedup:.2f}\n")
    
    # Tracer les courbes
    plt.figure(figsize=(10, 6))
    
    # Courbe de speedup
    plt.subplot(2, 1, 1)
    plt.plot(thread_counts, speedups, 'o-', label='Speedup mesuré')
    plt.plot(thread_counts, thread_counts, '--', label='Speedup idéal')
    plt.xlabel('Nombre de threads')
    plt.ylabel('Speedup')
    plt.title('Strong Scaling')
    plt.grid(True)
    plt.legend()
    plt.xticks(thread_counts)
    
    # Courbe des temps d'exécution
    plt.subplot(2, 1, 2)
    plt.plot(thread_counts, times, 'o-')
    plt.xlabel('Nombre de threads')
    plt.ylabel('Temps d\'exécution (s)')
    plt.title('Temps d\'exécution')
    plt.grid(True)
    plt.xticks(thread_counts)
    
    plt.tight_layout()
    plt.savefig(os.path.join(args.output_dir, 'scaling_plot.png'))
    
    print(f"\nRésultats sauvegardés dans {args.output_dir}")
    print(f"- Fichier de résultats: {results_file}")
    print(f"- Graphique: {os.path.join(args.output_dir, 'scaling_plot.png')}")

if __name__ == '__main__':
    main() 