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
    parser = argparse.ArgumentParser(description="Mesure du strong scaling - taille du problème fixe avec nombre de threads variable")
    
    # Arguments pour la configuration du benchmark
    parser.add_argument('--width', type=int, default=800,
                       help="Largeur de l'image")
    parser.add_argument('--height', type=int, default=800,
                       help="Hauteur de l'image")
    parser.add_argument('--samples', type=int, default=1000,
                       help="Nombre d'échantillons par pixel")
    parser.add_argument('--depth', type=int, default=5,
                       help="Profondeur maximale des rebonds")
    parser.add_argument('--max-threads', type=int, default=32,
                       help="Nombre maximum de threads à tester")
    parser.add_argument('--output-dir', type=str, default='benchmarks_strong_scaling',
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
    speedups = []

    print("Début des mesures de strong scaling...")
    print(f"Configuration: image {args.width}x{args.height}, {args.samples} échantillons/pixel")
    
    for threads in thread_counts:
        print(f"\nTest avec {threads} threads")
        
        output_file = os.path.join(args.output_dir, f'strong_scaling_threads_{threads}.ppm')
        
        exec_time = run_benchmark(
            args.width, args.height, args.samples, args.depth,
            threads, output_file
        )
        
        if exec_time is not None:
            times.append(exec_time)
            valid_threads.append(threads)
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

    # Calculer les speedups
    base_time = times[0]  # temps avec 1 thread
    speedups = [base_time / t for t in times]
    efficiency = [s / t * 100 for s, t in zip(speedups, valid_threads)]

    # Sauvegarder les résultats
    results_file = os.path.join(args.output_dir, 'strong_scaling_results.txt')
    with open(results_file, 'w') as f:
        f.write("Threads\tTime(s)\tSpeedup\tEfficiency(%)\n")
        for t, time, s, e in zip(valid_threads, times, speedups, efficiency):
            f.write(f"{t}\t{time:.2f}\t{s:.2f}\t{e:.2f}\n")

    # Créer le graphique
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))

    # Graphique des temps d'exécution
    ax1.plot(valid_threads, times, 'o-', color='tab:blue', label='Temps d\'exécution')
    ax1.set_xlabel('Nombre de threads')
    ax1.set_ylabel('Temps d\'exécution (s)')
    ax1.set_xscale('log', base=2)
    ax1.grid(True)
    ax1.set_xticks(valid_threads)
    ax1.set_xticklabels([str(t) for t in valid_threads])
    ax1.legend()

    # Graphique du speedup et de l'efficacité
    line1 = ax2.plot(valid_threads, speedups, 'o-', color='tab:green', label='Speedup')
    ax2.set_xlabel('Nombre de threads')
    ax2.set_ylabel('Speedup')
    ax2.grid(True)
    ax2.set_xscale('log', base=2)
    ax2.set_xticks(valid_threads)
    ax2.set_xticklabels([str(t) for t in valid_threads])

    # Ajouter la ligne de speedup idéal
    ideal_speedup = [t for t in valid_threads]
    ax2.plot(valid_threads, ideal_speedup, '--', color='gray', label='Speedup idéal')

    # Courbe d'efficacité sur l'axe de droite
    ax2_right = ax2.twinx()
    line2 = ax2_right.plot(valid_threads, efficiency, 'r--', label='Efficacité')
    ax2_right.set_ylabel('Efficacité (%)')
    ax2_right.set_ylim([0, 120])

    # Combiner les légendes du second graphique
    lines = line1 + line2
    labels = [l.get_label() for l in lines]
    ax2.legend(lines + [plt.Line2D([0], [0], linestyle='--', color='gray')],
               labels + ['Speedup idéal'], loc='center right')

    # Titre global
    fig.suptitle('Strong Scaling Performance\n(taille du problème fixe)', y=1.02)
    plt.tight_layout()

    # Sauvegarder le graphique
    plot_filename = os.path.join(args.output_dir, 'strong_scaling_plot.png')
    plt.savefig(plot_filename, bbox_inches='tight')

    print(f"\nRésultats sauvegardés dans {args.output_dir}")
    print(f"- Fichier de résultats: {results_file}")
    print(f"- Graphique: {plot_filename}")

if __name__ == '__main__':
    main() 