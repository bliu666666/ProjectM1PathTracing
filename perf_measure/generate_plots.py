#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Script de génération de graphiques pour l'analyse de performance CPU vs GPU
Complémentaire au script benchmark_speedup.sh
"""

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import os
import sys

def main():
    # Vérifier si le fichier CSV existe
    csv_file = "speedup_results.csv"
    if not os.path.exists(csv_file):
        print(f"Erreur: Le fichier {csv_file} n'existe pas.")
        print("Exécutez d'abord le script benchmark_speedup.sh")
        sys.exit(1)
    
    # Charger les données
    try:
        data = pd.read_csv(csv_file)
        print(f"Données chargées: {len(data)} entrées")
    except Exception as e:
        print(f"Erreur lors du chargement du CSV: {e}")
        sys.exit(1)
    
    # Créer le répertoire pour les graphiques
    os.makedirs("plots", exist_ok=True)
    
    # 1. Graphique de speedup
    plt.figure(figsize=(10, 6))
    plt.plot(data['Threads'], data['Speedup'], 'o-', linewidth=2, color='#1f77b4')
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.xlabel('Nombre de threads CPU')
    plt.ylabel('Speedup (CPU time / GPU time)')
    plt.title('Speedup GPU vs CPU en fonction du nombre de threads')
    
    # Ajouter une ligne horizontale à y=1 (parité CPU/GPU)
    plt.axhline(y=1, color='r', linestyle='--', alpha=0.7, label='Parité CPU/GPU')
    
    # Ajouter les valeurs sur les points
    for i, txt in enumerate(data['Speedup']):
        plt.annotate(f"{txt:.2f}x", 
                    (data['Threads'][i], data['Speedup'][i]),
                    textcoords="offset points", 
                    xytext=(0,10), 
                    ha='center')
    
    plt.legend()
    plt.tight_layout()
    plt.savefig('plots/speedup_graph.png', dpi=300)
    print("Graphique de speedup généré: plots/speedup_graph.png")
    
    # 2. Graphique des temps d'exécution
    plt.figure(figsize=(10, 6))
    
    # Créer un graphique à barres groupées
    bar_width = 0.35
    index = np.arange(len(data['Threads']))
    
    plt.bar(index, data['CPU_Time'], bar_width, label='CPU', color='#1f77b4', alpha=0.8)
    plt.bar(index + bar_width, data['GPU_Time'], bar_width, label='GPU', color='#ff7f0e', alpha=0.8)
    
    plt.xlabel('Nombre de threads CPU')
    plt.ylabel('Temps d\'exécution (secondes)')
    plt.title('Temps d\'exécution CPU vs GPU')
    plt.xticks(index + bar_width/2, data['Threads'])
    plt.legend()
    plt.grid(True, linestyle='--', alpha=0.7, axis='y')
    
    # Ajouter les valeurs sur les barres
    for i, v in enumerate(data['CPU_Time']):
        plt.text(i, v + 0.1, f"{v:.2f}s", ha='center')
    
    for i, v in enumerate(data['GPU_Time']):
        plt.text(i + bar_width, v + 0.1, f"{v:.2f}s", ha='center')
    
    plt.tight_layout()
    plt.savefig('plots/execution_time.png', dpi=300)
    print("Graphique des temps d'exécution généré: plots/execution_time.png")
    
    # 3. Graphique d'efficacité parallèle (pour CPU)
    plt.figure(figsize=(10, 6))
    
    # Calculer l'efficacité: speedup / nombre de threads
    # Normaliser par rapport au temps avec 1 thread
    base_time = data['CPU_Time'][0]
    speedup_vs_serial = [base_time / t for t in data['CPU_Time']]
    efficiency = [s / t for s, t in zip(speedup_vs_serial, data['Threads'])]
    
    plt.plot(data['Threads'], efficiency, 'o-', linewidth=2, color='#2ca02c')
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.xlabel('Nombre de threads CPU')
    plt.ylabel('Efficacité parallèle')
    plt.title('Efficacité parallèle CPU (speedup / nombre de threads)')
    
    # Ajouter une ligne horizontale à y=1 (efficacité idéale)
    plt.axhline(y=1, color='r', linestyle='--', alpha=0.7, label='Efficacité idéale')
    
    # Ajouter les valeurs sur les points
    for i, txt in enumerate(efficiency):
        plt.annotate(f"{txt:.2f}", 
                    (data['Threads'][i], efficiency[i]),
                    textcoords="offset points", 
                    xytext=(0,10), 
                    ha='center')
    
    plt.legend()
    plt.tight_layout()
    plt.savefig('plots/parallel_efficiency.png', dpi=300)
    print("Graphique d'efficacité parallèle généré: plots/parallel_efficiency.png")
    
    # 4. Graphique de performance relative (rayons par seconde)
    # Supposons que le nombre de rayons est proportionnel à width * height * samples
    resolution = data['Resolution'].iloc[0].replace(' ', '').split('x')
    width = int(resolution[0])
    height = int(resolution[1])
    samples = data['Samples'].iloc[0]
    
    total_rays = width * height * samples
    
    cpu_rays_per_sec = [total_rays / t for t in data['CPU_Time']]
    gpu_rays_per_sec = [total_rays / t for t in data['GPU_Time']]
    
    plt.figure(figsize=(10, 6))
    plt.plot(data['Threads'], cpu_rays_per_sec, 'o-', linewidth=2, label='CPU', color='#1f77b4')
    plt.plot(data['Threads'], gpu_rays_per_sec, 'o-', linewidth=2, label='GPU', color='#ff7f0e')
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.xlabel('Nombre de threads CPU')
    plt.ylabel('Rayons par seconde')
    plt.title('Performance CPU vs GPU (rayons par seconde)')
    plt.legend()
    
    # Utiliser une échelle logarithmique si la différence est grande
    if max(gpu_rays_per_sec) / min(cpu_rays_per_sec) > 10:
        plt.yscale('log')
        plt.ylabel('Rayons par seconde (échelle log)')
    
    plt.tight_layout()
    plt.savefig('plots/rays_per_second.png', dpi=300)
    print("Graphique de performance relative généré: plots/rays_per_second.png")
    
    print("\nTous les graphiques ont été générés dans le répertoire 'plots/'")

if __name__ == "__main__":
    main()
