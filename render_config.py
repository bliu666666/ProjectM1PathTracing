#!/usr/bin/env python3

import argparse
import subprocess
import os
import shutil
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
    parser = argparse.ArgumentParser(description='Configurer et lancer le rendu du ray tracer')
    
    # Arguments pour la configuration du rendu
    parser.add_argument('--width', type=int, default=800, help='Largeur de l\'image (défaut: 800)')
    parser.add_argument('--height', type=int, default=600, help='Hauteur de l\'image (défaut: 600)')
    parser.add_argument('--samples', type=int, default=100, help='Nombre d\'échantillons par pixel (défaut: 100)')
    parser.add_argument('--depth', type=int, default=50, help='Profondeur maximale des rebonds (défaut: 50)')
    parser.add_argument('--threads', type=int, default=0, help='Nombre de threads (0 = auto, défaut: 0)')
    parser.add_argument('--output', type=str, default='images/output.ppm', help='Chemin du fichier de sortie (défaut: images/output.ppm)')
    
    args = parser.parse_args()
    
    # S'assurer que le dossier images existe
    images_dir = 'images'
    if not os.path.exists(images_dir):
        os.makedirs(images_dir)
    
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
    
    
    # Déterminer le nom de fichier final
    final_output = get_next_output_filename(args.output)
    
    # Lancer le rendu avec les paramètres spécifiés
    print(f"Lancement du rendu avec les paramètres suivants:")
    print(f"- Résolution: {args.width}x{args.height}")
    print(f"- Échantillons par pixel: {args.samples}")
    print(f"- Profondeur maximale: {args.depth}")
    print(f"- Nombre de threads: {'auto' if args.threads == 0 else args.threads}")
    print(f"- Fichier de sortie: {final_output}")
    
    # Construire la liste des arguments
    cmd_args = ['./build/RayTracer']
    if args.width != 800:
        cmd_args.extend(['-w', str(args.width)])
    if args.height != 600:
        cmd_args.extend(['-h', str(args.height)])
    if args.samples != 100:
        cmd_args.extend(['-s', str(args.samples)])
    if args.depth != 50:
        cmd_args.extend(['-d', str(args.depth)])
    if args.threads != 0:
        cmd_args.extend(['-t', str(args.threads)])
    cmd_args.extend(['-o', final_output])
    
    try:
        # Exécuter le programme avec les arguments
        subprocess.run(cmd_args, check=True)
        
        # Vérifier si le fichier a été créé
        if os.path.exists(final_output):
            print(f"Rendu terminé avec succès. Fichier sauvegardé sous: {final_output}")
        else:
            print("Erreur: Le fichier de sortie n'a pas été créé.")
            
    except subprocess.CalledProcessError as e:
        print(f"Erreur lors de l'exécution: {e}")
    except FileNotFoundError:
        print("Erreur: L'exécutable RayTracer n'a pas été trouvé. Vérifiez la compilation.")

if __name__ == '__main__':
    main() 