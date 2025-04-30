# Guide de Profiling avec Docker

Ce guide vous explique comment utiliser les outils de profiling dans le conteneur Docker pour analyser les performances de votre code de PathTracing.

## Outils disponibles

Le conteneur Docker contient plusieurs outils de profiling :

1. **gprof** - Profilage standard compatible avec gcc
2. **Valgrind/Callgrind** - Profilage détaillé des appels de fonctions
3. **Google Performance Tools** - Profilage avancé avec faible impact sur la performance

## Instructions d'utilisation

### 1. Démarrer le conteneur Docker

```bash
./run_profiling_docker.sh
```

### 2. Utiliser gprof

Dans le conteneur Docker, exécutez :

```bash
# Configuration de base (largeur, hauteur, échantillons, fichier de sortie, profondeur)
run_profiling.sh 800 600 100 output.ppm 50
```

Le rapport de profiling sera généré dans `/app/build/profiling_report.txt`.

### 3. Utiliser Callgrind

```bash
# Installation de la dépendance graphviz si nécessaire
apt-get update && apt-get install -y graphviz

# Exécution du profiling avec Callgrind
run_callgrind.sh 800 600 10 output.ppm 10
```

Cela générera une visualisation dans `/app/build/callgrind_profile.png`.

### 4. Utiliser Google Performance Tools

```bash
run_gperftools.sh 800 600 10 output.ppm 10
```

### 5. Pour les programmes interactifs

Si votre programme nécessite des entrées utilisateur, vous pouvez utiliser ce script pour les automatiser :

```bash
cat > run_with_inputs.sh << 'EOF'
#!/bin/bash

# Lancer le programme et lui fournir les entrées via un script
{
    # Attendre que le programme demande l'origine de la caméra
    sleep 0.1
    echo "0 0 0"
    
    # Attendre que le programme demande le point de visée
    sleep 0.1
    echo "0 0 -1"
    
    # Attendre que le programme demande le vecteur up
    sleep 0.1
    echo "0 1 0"
    
    # Attendre que le programme demande le champ de vision
    sleep 0.1
    echo "90"
} | $1 $2 $3 $4 $5 $6
EOF

chmod +x run_with_inputs.sh
```

Puis utilisez-le avec n'importe quel outil de profiling, par exemple :

```bash
CPUPROFILE=/app/build/cpu.prof LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libprofiler.so.0 ./run_with_inputs.sh ./render 800 600 10 output.ppm 10
```

## Analyse des résultats

### gprof

Le rapport gprof montre les fonctions qui consomment le plus de temps CPU. Cherchez les fonctions avec les plus grands pourcentages dans la colonne "% time".

### Callgrind

L'image générée montre un graphe d'appels avec le temps passé dans chaque fonction. Les nœuds les plus gros et les plus foncés sont les points chauds à optimiser.

### Google Performance Tools

Le rapport textuel et PDF montre les fonctions consommant le plus de ressources. C'est généralement l'outil le plus précis pour les applications modernes.

## Conseils pour l'optimisation

1. Concentrez-vous sur les fonctions qui apparaissent en haut des rapports de profiling
2. Cherchez les opportunités de parallélisation dans les sections qui prennent le plus de temps
3. Optimisez les algorithmes critiques avant d'optimiser le code
4. Considérez l'utilisation de structures de données plus efficaces
5. Pour la parallélisation, cherchez des boucles indépendantes qui peuvent être exécutées en parallèle
