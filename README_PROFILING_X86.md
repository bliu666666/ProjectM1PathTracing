# Profiling du PathTracer sur architecture x86 virtualisée

Ce document explique comment profiler votre code PathTracing sur une architecture x86 virtualisée avec gprof, même si vous utilisez un Mac M3.

## Pourquoi utiliser Docker avec une architecture x86 virtualisée ?

Les puces Apple Silicon (M1, M2, M3) fonctionnent sur une architecture ARM, ce qui peut poser des problèmes de compatibilité avec certains outils comme gprof. En utilisant Docker avec l'architecture x86 virtualisée, nous obtenons plusieurs avantages :

1. **Environnement stable et cohérent** - L'environnement Linux x86 est largement testé avec gprof
2. **Compatibilité garantie** - Élimine les problèmes spécifiques à l'architecture ARM
3. **Aucune installation requise** - Tous les outils sont installés dans le conteneur

## Comment ça marche ?

1. Nous créons une image Docker basée sur Ubuntu x86_64
2. Docker traduit automatiquement les instructions ARM vers x86 (virtualisation)
3. Nous compilons et exécutons le code dans cet environnement x86 virtualisé
4. gprof fonctionne parfaitement dans cet environnement

## Utilisation

Pour profiler votre code :

```bash
./run_gprof_x86.sh
```

Vous pouvez personnaliser les paramètres de rendu :

```bash
./run_gprof_x86.sh [largeur] [hauteur] [échantillons] [fichier_sortie] [profondeur_max]
```

Par exemple :

```bash
./run_gprof_x86.sh 1024 768 100 high_quality.ppm 50
```

Pour analyser les résultats de profiling :

```bash
./analyze_profiling.sh
```

## Configuration technique

- **Architecture virtualisée** : x86_64 (amd64)
- **Système d'exploitation** : Ubuntu 22.04
- **Outil de profiling principal** : gprof
- **Compilation** : Utilise l'option ENABLE_PROFILING de votre CMakeLists.txt

## Comment interpréter les résultats

Le rapport gprof (build/profile_report.txt) contient plusieurs sections importantes :

1. **Flat profile** - Montre quelles fonctions ont consommé le plus de temps
   - % time : Pourcentage du temps total
   - self seconds : Temps passé dans la fonction elle-même
   - calls : Nombre d'appels à la fonction

2. **Call graph** - Montre qui appelle qui et combien de temps est passé dans chaque fonction
   - Les fonctions sont regroupées par relations parent/enfant
   - Les pourcentages indiquent où concentrer vos efforts d'optimisation

## Étapes suivantes

1. **Identifier les points chauds** - Quelles fonctions consomment le plus de temps ?
2. **Analyser les dépendances** - Ces fonctions peuvent-elles être parallélisées ?
3. **Optimiser** - Appliquer des optimisations algorithmiques ou de parallélisation
4. **Mesurer à nouveau** - Exécuter le profiling après optimisation pour mesurer les gains

## Conseils pour la parallélisation

Les fonctions qui sont de bonnes candidates pour la parallélisation sont souvent :

1. Les traitements par pixel indépendants
2. Les calculs d'intersection de rayons
3. Les calculs d'échantillonnage multiples
4. Toute boucle avec des itérations indépendantes
