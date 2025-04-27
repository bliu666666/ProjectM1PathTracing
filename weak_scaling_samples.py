
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

    print(f"  Lancement: {' '.join(cmd_args)}") 
    start_time = time.time()
    try:
        subprocess.run(cmd_args, check=True, text=True) 
    except subprocess.CalledProcessError as e:
        print(f"Erreur lors de l'exécution pour {samples} échantillons: {e}")
        print(f"STDOUT: {e.stdout}")
        print(f"STDERR: {e.stderr}")
        return None 
    end_time = time.time()

    return end_time - start_time

def main():
    

    parser = argparse.ArgumentParser(description="Mesurer les performances selon le nombre d'échantillons (taille du problème)")

    # Arguments pour la configuration du benchmark
    parser.add_argument('--width', type=int, default=1920, help="Largeur de l'image ")
    parser.add_argument('--height', type=int, default=1080, help="Hauteur de l'image ")
    parser.add_argument('--depth', type=int, default=50, help="Profondeur maximale des rebonds ")
    parser.add_argument('--threads', type=int, default=16, help=f"Nombre fixe de threads à utiliser (défaut: 16)")
    parser.add_argument('--output-dir', type=str, default='benchmarks_weak_scaling', help="Dossier pour sauvegarder les résultats")

    args = parser.parse_args()

    # Créer le dossier de sortie s'il n'existe pas
    if not os.path.exists(args.output_dir):
        os.makedirs(args.output_dir)

    # Nombre d'échantillons fixe à tester
    sample_counts = [100, 250, 500, 1000, 2000]

    # Mesures pour différents nombres d'échantillons
    times = []
    valid_sample_counts = [] # Garde uniquement les tests réussis

    print(f"Début des mesures pour {args.threads} threads...")
    for samples in sample_counts:
        print(f"\nTest avec {samples} échantillons par pixel...")
        output_file = get_next_output_filename(os.path.join(args.output_dir, f'samples_{samples}_threads_{args.threads}.ppm'))

        exec_time = run_benchmark(
            args.width, args.height, samples, args.depth,
            args.threads, output_file
        )

        if exec_time is not None:
            times.append(exec_time)
            valid_sample_counts.append(samples) # Ajoute aux échantillons valides
            print(f"Temps d'exécution: {exec_time:.2f}s")
        else:
             print(f"Échec du test pour {samples} échantillons. Ignoré.")

    # Vérifier si des tests ont réussi
    if not times:
        print("Aucun test n'a réussi. Impossible de générer les résultats.")
        return

    # Sauvegarder les résultats
    results_file = os.path.join(args.output_dir, f'results_threads_{args.threads}.txt')
    with open(results_file, 'w') as f:
        f.write("Samples\tTime(s)\n")
        for s, t in zip(valid_sample_counts, times):
            f.write(f"{s}\t{t:.2f}\n")

    # Tracer la courbe des temps d'exécution
    plt.figure(figsize=(10, 6))
    plt.plot(valid_sample_counts, times, 'o-')
    plt.xlabel('Nombre d\'échantillons par pixel')
    plt.ylabel('Temps d\'exécution (s)')
    plt.title(f'Performance selon le nombre d\'échantillons ({args.threads} threads)')
    plt.grid(True)
    plt.xticks(valid_sample_counts, labels=[str(s) for s in valid_sample_counts]) 

    plt.tight_layout()
    plot_filename = os.path.join(args.output_dir, f'scaling_samples_plot_threads_{args.threads}.png')
    plt.savefig(plot_filename)

    print(f"\nRésultats sauvegardés dans {args.output_dir}")
    print(f"- Fichier de résultats: {results_file}")
    print(f"- Graphique: {plot_filename}")

if __name__ == '__main__':
    main() 