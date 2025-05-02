#!/usr/bin/env python3

import argparse
import subprocess
import os
import time
import matplotlib.pyplot as plt
import numpy as np

def run_benchmark(width, height, samples, depth, threads, output):
    """Exécute le ray tracer avec les paramètres donnés et retourne le temps d'exécution."""
    cmd = ['./build/render',
           str(width),
           str(height),
           str(samples),
           output,
           str(depth),
           '0',  # mode normal
           str(threads),  # nombre de threads
           '8']  # chunk size fixe
    
    print(f"  Lancement avec {threads} threads: {' '.join(cmd)}")
    start_time = time.time()
    try:
        subprocess.run(cmd, check=True, capture_output=True, text=True)
    except subprocess.CalledProcessError as e:
        print(f"Erreur lors de l'exécution avec {threads} threads: {e}")
        print(f"STDOUT: {e.stdout}")
        print(f"STDERR: {e.stderr}")
        return None
    end_time = time.time()
    
    return end_time - start_time

def main():
    parser = argparse.ArgumentParser(description="Mesure du weak scaling - augmente la taille de l'image proportionnellement au nombre de threads")
    
    # Arguments pour la configuration du benchmark
    parser.add_argument('--base-width', type=int, default=200,
                       help="Largeur de base de l'image pour 1 thread")
    parser.add_argument('--base-height', type=int, default=200,
                       help="Hauteur de base de l'image pour 1 thread")
    parser.add_argument('--samples', type=int, default=100,
                       help="Nombre d'échantillons par pixel")
    parser.add_argument('--depth', type=int, default=5,
                       help="Profondeur maximale des rebonds")
    parser.add_argument('--max-threads', type=int, default=32,
                       help="Nombre maximum de threads à tester")
    parser.add_argument('--output-dir', type=str, default='benchmarks_weak_scaling',
                       help="Dossier pour sauvegarder les résultats")

    args = parser.parse_args()

    # Créer le dossier de sortie s'il n'existe pas
    if not os.path.exists(args.output_dir):
        os.makedirs(args.output_dir)

    # Liste des nombres de threads à tester (puissances de 2)
    thread_counts = [2**i for i in range(0, args.max_threads.bit_length())]
    if thread_counts[-1] > args.max_threads:
        thread_counts = thread_counts[:-1]
    if 1 not in thread_counts:
        thread_counts.insert(0, 1)

    # Mesures pour différents nombres de threads
    times = []
    valid_threads = []
    sizes = []

    print("Début des mesures de weak scaling...")
    print(f"Configuration de base: image {args.base_width}x{args.base_height}, {args.samples} échantillons/pixel")
    
    for threads in thread_counts:
        # Calculer la taille de l'image proportionnelle au nombre de threads
        width = int(args.base_width * np.sqrt(threads))
        height = int(args.base_height * np.sqrt(threads))
        print(f"\nTest avec {threads} threads - Image: {width}x{height}")
        
        output_file = os.path.join(args.output_dir, f'weak_scaling_threads_{threads}.ppm')
        
        exec_time = run_benchmark(
            width, height, args.samples, args.depth,
            threads, output_file
        )
        
        if exec_time is not None:
            times.append(exec_time)
            valid_threads.append(threads)
            sizes.append(width * height)
            print(f"Temps d'exécution: {exec_time:.2f}s")
            
            # Supprimer le fichier de sortie pour économiser de l'espace
            if os.path.exists(output_file):
                os.remove(output_file)
        else:
            print(f"Échec du test pour {threads} threads. Ignoré.")

    # Vérifier si des tests ont réussi
    if not times:
        print("Aucun test n'a réussi. Impossible de générer les résultats.")
        return

    # Sauvegarder les résultats
    results_file = os.path.join(args.output_dir, 'weak_scaling_results.txt')
    with open(results_file, 'w') as f:
        f.write("Threads\tSize\tTime(s)\n")
        for t, s, time in zip(valid_threads, sizes, times):
            f.write(f"{t}\t{s}\t{time:.2f}\n")

    # Tracer la courbe des temps d'exécution
    fig, ax1 = plt.subplots(figsize=(10, 6))
    
    # Courbe du temps d'exécution (axe gauche)
    line1 = ax1.plot(valid_threads, times, 'o-', color='tab:blue', label='Temps d\'exécution')
    ax1.set_xlabel('Nombre de threads')
    ax1.set_ylabel('Temps d\'exécution (s)')
    ax1.set_xscale('log', base=2)
    ax1.grid(True)
    ax1.set_xticks(valid_threads)
    ax1.set_xticklabels([str(t) for t in valid_threads])

    # Courbe d'efficacité (axe droit)
    ax2 = ax1.twinx()
    base_time = times[0]
    efficiency = [base_time / t * 100 for t in times]
    line2 = ax2.plot(valid_threads, efficiency, 'r--', label='Efficacité')
    ax2.set_ylabel('Efficacité (%)')
    ax2.set_ylim([0, 120])

    # Titre
    plt.title('Weak Scaling Performance\n(taille du problème (dimension) augmente avec le nombre de threads)')

    # Légende unique combinant les deux courbes
    lines = line1 + line2
    labels = [l.get_label() for l in lines]
    ax1.legend(lines, labels, loc='upper right')

    plt.tight_layout()
    plot_filename = os.path.join(args.output_dir, 'weak_scaling_plot.png')
    plt.savefig(plot_filename)

    print(f"\nRésultats sauvegardés dans {args.output_dir}")
    print(f"- Fichier de résultats: {results_file}")
    print(f"- Graphique: {plot_filename}")

if __name__ == '__main__':
    main() 