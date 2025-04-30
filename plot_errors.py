#!/usr/bin/env python3

import subprocess
import re
import matplotlib.pyplot as plt
import numpy as np

def extract_error(output):
    """Extrait l'erreur MSE de la sortie du script."""
    match = re.search(r"Erreur MSE: (\d+\.\d+)", output)
    if match:
        return float(match.group(1))
    return None

def main():
    # Nombre d'échantillons à comparer
    samples = [100, 200, 300, 400, 500, 600, 700, 800, 900, 1000]
    errors = []
    
    # Fichier de référence
    reference = "images/rendu20000-400-300.ppm"
    
    print("Début des comparaisons...")
    
    # Exécuter les comparaisons
    for n in samples:
        test_file = f"images/rendu{n}-400-300.ppm"
        print(f"Comparaison avec {test_file}...")
        
        try:
            result = subprocess.run(
                ["python3", "./compare_images.py", reference, test_file],
                capture_output=True,
                text=True
            )
            
            error = extract_error(result.stdout)
            if error is not None:
                errors.append(error)
                print(f"Erreur MSE: {error:.6f}")
            else:
                print(f"Erreur lors de l'extraction de l'erreur pour {n} échantillons")
                
        except Exception as e:
            print(f"Erreur lors de la comparaison pour {n} échantillons: {e}")
    
    if not errors:
        print("Aucune donnée n'a été collectée")
        return
    
    # Créer le graphique
    plt.figure(figsize=(10, 6))
    plt.plot(samples[:len(errors)], errors, 'o-', linewidth=2, markersize=8)
    plt.xlabel('Nombre d\'échantillons')
    plt.ylabel('Erreur MSE')
    plt.title('Évolution de l\'erreur MSE en fonction du nombre d\'échantillons')
    plt.grid(True)
    
    # Sauvegarder le graphique
    plt.savefig('images/sampling_MSE.png')
    print("\nGraphique sauvegardé dans 'images/sampling_MSE.png'")
    
    # Afficher les données
    print("\nRésumé des erreurs:")
    for n, err in zip(samples[:len(errors)], errors):
        print(f"{n} échantillons: {err:.6f}")

if __name__ == '__main__':
    main() 