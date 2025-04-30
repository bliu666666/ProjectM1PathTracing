#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np

# Données du fichier temps-scaling.txt
samples = [100, 200, 300, 400, 500, 600, 700, 800, 900, 1000]
times = [96.94, 193.00, 251.53, 343.40, 436.17, 551.00, 637.63, 683.59, 420.19, 471.68]

# Créer le graphique
plt.figure(figsize=(12, 6))

# Tracer les points et la ligne
plt.plot(samples, times, 'o-', linewidth=2, markersize=8)

# Labels et titre
plt.xlabel('Nombre d\'échantillons')
plt.ylabel('Temps d\'exécution (secondes)')
plt.title('Évolution du temps d\'exécution en fonction du nombre d\'échantillons')
plt.grid(True)

# Sauvegarder le graphique
plt.savefig('images/time_scaling.png')
print("\nGraphique sauvegardé dans 'images/time_scaling.png'")

# Afficher les données
print("\nRésumé des temps d'exécution:")
for n, t in zip(samples, times):
    print(f"{n:5d} échantillons: {t:.2f} secondes") 