#!/usr/bin/env python3

import numpy as np
import argparse
import os

def read_ppm(filename):
    """Lit un fichier PPM au format P3 (ASCII) et retourne un tableau numpy des pixels."""
    with open(filename, 'r') as f:
        # Lire l'en-tête PPM
        header = f.readline().strip()
        if header != 'P3':
            raise ValueError("Format PPM non supporté (doit être P3)")
        
        # Lire les dimensions
        while True:
            line = f.readline().strip()
            if not line.startswith('#'):
                break
        width, height = map(int, line.split())
        
        # Lire la valeur maximale
        max_val = int(f.readline().strip())
        
        # Lire les données ASCII
        data = []
        for line in f:
            if line.strip().startswith('#'):
                continue
            # Convertir les valeurs en float et normaliser par max_val
            values = [float(x) / max_val for x in line.split()]
            data.extend(values)
        
        # Convertir en tableau numpy et redimensionner
        data = np.array(data, dtype=np.float32)
        data = data.reshape((height, width, 3))
        
        return data

def calculate_mse(ref_image, test_image):
    """Calcule l'erreur quadratique moyenne (MSE) entre deux images."""
    # Vérifier que les images ont les mêmes dimensions
    if ref_image.shape != test_image.shape:
        raise ValueError("Les images ont des dimensions différentes")
    
    # Calculer la différence au carré
    squared_diff = np.square(test_image - ref_image)
    
    # Calculer la moyenne sur les trois canaux
    mean_squared_diff = np.mean(squared_diff, axis=2)
    
    # Calculer la moyenne globale
    mse = np.mean(mean_squared_diff)
    
    return mse

def main():
    parser = argparse.ArgumentParser(description='Compare deux images PPM et calcule l\'erreur MSE')
    parser.add_argument('reference', help='Image de référence PPM')
    parser.add_argument('test', help='Image à comparer PPM')
    parser.add_argument('--output', help='Fichier de sortie pour l\'erreur (optionnel)')
    
    args = parser.parse_args()
    
    # Vérifier que les fichiers existent
    if not os.path.exists(args.reference):
        print(f"Erreur: Le fichier de référence {args.reference} n'existe pas")
        return
    if not os.path.exists(args.test):
        print(f"Erreur: Le fichier de test {args.test} n'existe pas")
        return
    
    # Lire les images
    try:
        ref_image = read_ppm(args.reference)
        test_image = read_ppm(args.test)
    except Exception as e:
        print(f"Erreur lors de la lecture des images: {e}")
        return
    
    # Vérifier que les dimensions correspondent
    if ref_image.shape != test_image.shape:
        print(f"Erreur: Les images ont des dimensions différentes")
        print(f"Référence: {ref_image.shape}, Test: {test_image.shape}")
        return
    
    # Calculer l'erreur MSE
    error = calculate_mse(ref_image, test_image)
    
    # Afficher les statistiques
    print("\nStatistiques de l'erreur:")
    print(f"Erreur MSE: {error:.6f}")
    
    # Sauvegarder l'erreur si demandé
    if args.output:
        np.save(args.output, error)
        print(f"\nCarte d'erreur sauvegardée dans {args.output}")

if __name__ == '__main__':
    main() 