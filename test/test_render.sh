#!/bin/bash

# Parameter settings
resolutions=("200x100" "400x200" "800x400" "1600x800")
samples=("10" "50" "100" "1000")
depths=("10" "50")

# Iterate over parameter combinations
for res in "${resolutions[@]}"; do
    for sample in "${samples[@]}"; do
        for depth in "${depths[@]}"; do
            width=$(echo $res | cut -d'x' -f1)
            height=$(echo $res | cut -d'x' -f2)
            output="output_${width}x${height}_${sample}_${depth}.ppm"
            echo "Running render with resolution=${width}x${height}, samples=${sample}, depth=${depth}"
            ../build/render $width $height $sample $output $depth
            rm output*.ppm
        done
    done
done
