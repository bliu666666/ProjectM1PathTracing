#!/usr/bin/env python3

import argparse
import subprocess
import os
import shutil
import time
from pathlib import Path

def get_next_output_filename(base_path):
    """Génère le prochain nom de fichier disponible."""
    base_dir = os.path.dirname(base_path)
    base_name = os.path.basename(base_path)
    name, ext = os.path.splitext(base_name)
    
    counter = 1
    while True:
        if counter == 1:
            new_name = f"{name}{ext}"
        else:
            new_name = f"{name}{counter}{ext}"
        
        new_path = os.path.join(base_dir, new_name)
        if not os.path.exists(new_path):
            return new_path
        counter += 1

def main():
    parser = argparse.ArgumentParser(description='Configurer et lancer le path tracer')
    
    # Arguments pour la configuration du rendu
    parser.add_argument('--width', type=float, default=800, help='Largeur de l\'image (défaut: 800)')
    parser.add_argument('--height', type=float, default=600, help='Hauteur de l\'image (défaut: 600)')
    parser.add_argument('--samples', type=int, default=100, help='Nombre d\'échantillons par pixel (défaut: 100)')
    parser.add_argument('--depth', type=int, default=10, help='Profondeur maximale des rebonds (défaut: 10)')
    parser.add_argument('--mode', type=int, default=0, help='Mode de rendu (0: normal, 1: MLT, défaut: 0)')
    parser.add_argument('--iterations', type=int, default=1000, help='Nombre d\'itérations MLT (défaut: 1000)')
    parser.add_argument('--output', type=str, default='output/output.ppm', help='Chemin du fichier de sortie (défaut: output/output.ppm)')
    parser.add_argument('--threads', type=int, default=16, help='Nombre de threads à utiliser (défaut: nombre de cœurs CPU)')
    
    args = parser.parse_args()
    
    # S'assurer que le dossier output existe
    output_dir = 'output'
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    
    # Vérifier si le dossier build existe
    if not os.path.exists('build'):
        os.makedirs('build')
    
    # Vérifier si CMakeLists.txt existe
    if not os.path.exists('CMakeLists.txt'):
        print("Erreur: CMakeLists.txt non trouvé")
        return
    
    # Compiler le projet si nécessaire
    print("Compilation du projet...")
    try:
        subprocess.run(['cmake', '-B', 'build', '-S', '.'], check=True)
        subprocess.run(['make', '-C', 'build'], check=True)
    except subprocess.CalledProcessError as e:
        print(f"Erreur lors de la compilation: {e}")
        return
    
    # Déterminer le nom de fichier final
    final_output = get_next_output_filename(args.output)
    
    # Lancer le rendu avec les paramètres spécifiés
    print(f"Lancement du rendu avec les paramètres suivants:")
    print(f"- Résolution: {args.width}x{args.height}")
    print(f"- Échantillons par pixel: {args.samples}")
    print(f"- Profondeur maximale: {args.depth}")
    print(f"- Mode de rendu: {'Normal' if args.mode == 0 else 'MLT'}")
    print(f"- Nombre de threads: {args.threads}")
    if args.mode == 1:
        print(f"- Itérations MLT: {args.iterations}")
    print(f"- Fichier de sortie: {final_output}")
    
    try:
        # Construire la commande
        cmd = ['./build/render', 
               str(args.width), 
               str(args.height), 
               str(args.samples), 
               final_output, 
               str(args.depth), 
               str(args.mode),
               str(args.threads)]
        
        # Mesurer le temps d'exécution
        start_time = time.time()
        
        # Exécuter le programme
        process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, stderr = process.communicate()
        
        end_time = time.time()
        execution_time = end_time - start_time
        
        if process.returncode == 0:
            print(f"Rendu terminé avec succès. Fichier sauvegardé sous: {final_output}")
            print(f"Temps d'exécution: {execution_time:.2f} secondes")
        else:
            print(f"Erreur lors de l'exécution: {stderr.decode()}")
            
    except FileNotFoundError:
        print("Erreur: L'exécutable render n'a pas été trouvé. Vérifiez la compilation.")
    except Exception as e:
        print(f"Erreur inattendue: {e}")

if __name__ == '__main__':
    main() 