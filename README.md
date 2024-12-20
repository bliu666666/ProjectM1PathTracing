# Path Tracing Renderer

Ce projet est une implémentation d'un moteur de rendu utilisant la technique du path tracing, développé dans le cadre du Master 1. Le path tracing est une méthode de rendu physiquement réaliste qui simule le comportement de la lumière dans une scène 3D.

## Fonctionnalités

- Rendu de scènes 3D avec path tracing
- Support pour les objets géométriques :
  - Sphères
  - Boîtes alignées sur les axes (AABB)
- Matériaux diffus (Lambertian)
- Antialiasing par échantillonnage multiple
- Correction gamma
- Export au format PPM

## Prérequis

- CMake (version 3.10 minimum)
- Compilateur C++ supportant C++11
- Python 3.x (pour le script de conversion PPM)

## Installation

1. Clonez le dépôt :
```bash
git clone [votre-repo]
cd ProjectM1PathTracing
```

2. Créez et accédez au répertoire de build :
```bash
mkdir build
cd build
```

3. Générez les fichiers de build avec CMake :
```bash
cmake ..
```

4. Compilez le projet :
```bash
make
```

## Utilisation

Le programme prend plusieurs arguments en ligne de commande :
```bash
./PathTracer [largeur] [hauteur] [échantillons_par_pixel] [chemin_sortie] [profondeur_max]
```

Exemple :
```bash
./PathTracer 800 600 100 output.ppm 50
```

Après l'exécution, vous serez invité à entrer les paramètres de la caméra :
- Position de la caméra (x,y,z)
- Point visé (x,y,z)
- Vecteur haut (x,y,z)
- Champ de vision vertical (en degrés)

### Conversion du fichier PPM

Un script Python est fourni pour convertir les fichiers PPM en d'autres formats :
```bash
python3 convert_ppm.py output.ppm output.png
```

## Structure du Code

- `src/` : Contient les fichiers source
  - `render.h/.cpp` : Fonctions principales de rendu
  - `sphere.h/.cpp` : Implémentation des sphères
  - `obj.h` : Classe de base pour les objets
  - `material.h` : Classe de base pour les matériaux
  - `lambertian.h/.cpp` : Implémentation du matériau diffus
  - `vec3.h/.cpp` : Classe pour les vecteurs 3D
  - `ray.h/.cpp` : Classe pour les rayons
  - `camera.h/.cpp` : Gestion de la caméra

## Architecture du Projet

Le projet utilise une architecture orientée objet avec :
1. Une hiérarchie d'objets géométriques (Object -> Sphere, AABB)
2. Une hiérarchie de matériaux (Material -> Lambertian)
3. Un système de lancer de rayons récursif pour le path tracing

## Limitations Actuelles

- Pas de sources de lumière explicites (utilise uniquement l'éclairage ambiant)
- Pas de support pour les matériaux réfléchissants ou réfractifs
- Pas d'optimisation par structures accélératrices (BVH, etc.)
