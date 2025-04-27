# Vérifier si le build existe 
if [ ! -f ./build/RayTracer ]; then
    echo "Erreur: L'exécutable ./build/RayTracer n'a pas été trouvé."
    echo "Veuillez compiler le projet C++ avant de lancer les benchmarks."
    exit 1
fi

# Lancer le benchmark de Strong Scaling
echo "=================================================="
echo " Lancement du benchmark Strong Scaling...        "
echo "=================================================="
python3 strong_scaling.py

# Vérifier le code de sortie 
if [ $? -ne 0 ]; then
    echo "Erreur lors de l'exécution de strong_scaling.py"
    # exit 1 # Décommentez pour arrêter le script en cas d'erreur
fi

echo ""
echo "========================================================"
echo " Lancement du benchmark Weak Scaling (par Samples)... "
echo "========================================================"
python3 weak_scaling_samples.py

# Vérifier le code de sortie 
if [ $? -ne 0 ]; then
    echo "Erreur lors de l'exécution de weak_scaling_samples.py"
    # exit 1 # Décommentez pour arrêter le script en cas d'erreur
fi

echo ""
echo "==========================="
echo " Tous les benchmarks sont terminés. "
echo "===========================" 