## Interprétation des résultats

### Rapport gprof

Le rapport gprof montre les fonctions qui consomment le plus de temps CPU. Les colonnes principales sont :

- **% time** : Pourcentage du temps d'exécution passé dans la fonction
- **cumulative seconds** : Temps cumulé passé dans la fonction et toutes les fonctions qu'elle appelle
- **self seconds** : Temps passé uniquement dans la fonction elle-même
- **calls** : Nombre d'appels à la fonction

### Localisation des résultats

Tous les résultats de profiling sont sauvegardés dans le répertoire `profiling_results/` :

- **profile_report.txt** : Rapport complet généré par gprof
- **gmon.out** : Fichier de données brutes du profiling
- **output.ppm** : Image générée pendant le profiling

## Stratégies d'optimisation

### Points chauds à rechercher

1. **Calculs mathématiques intensifs** : Opérations trigonométriques, racines carrées
2. **Boucles imbriquées** : Souvent dans le traitement des pixels ou des rayons
3. **Calculs redondants** : Valeurs qui pourraient être calculées une seule fois
4. **Intersections d'objets** : Souvent coûteuses dans un path tracer

### Techniques d'optimisation recommandées

1. **Parallélisation** :
   - OpenMP pour paralléliser les boucles (`#pragma omp parallel for`)
   - Thread pools pour traiter les images par blocs

2. **Optimisations algorithmiques** :
   - Structures d'accélération (BVH, KD-tree) pour les intersections
   - Précalcul de valeurs fréquemment utilisées

3. **Optimisations bas niveau** :
   - Utilisation de SIMD (AVX/SSE) pour les opérations vectorielles
   - Optimisation de la mémoire cache (alignement, localité)

## Exemple d'optimisation OpenMP

Après avoir identifié une boucle coûteuse via profiling, vous pouvez l'optimiser ainsi :

```cpp
// Avant optimisation
for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
        // Calcul de pixel coûteux
    }
}

// Après optimisation avec OpenMP
#pragma omp parallel for collapse(2)
for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
        // Même calcul, maintenant en parallèle
    }
}
```

## Conclusion

The profiling is the first step crucial for optimizing your code. Identify first the hotspots, then apply targeted optimizations. Always measure the impact of your optimizations with a new profiling.