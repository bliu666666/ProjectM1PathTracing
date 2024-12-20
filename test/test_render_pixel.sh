#!/bin/bash

# Parameter settings
resolutions=("800x800")
depths=("50")
samples=("10" "50" "100" "1000")

echo "pixel,Time(s)" > render_pixel_results.csv

for res in "${resolutions[@]}"; do
    for depth in "${depths[@]}"; do
        for sample in "${samples[@]}"; do
            width=$(echo $res | cut -d'x' -f1)
            height=$(echo $res | cut -d'x' -f2)
            output="output_${width}x${height}_${sample}_${depth}.ppm"
            start_time=$(date +%s.%N)
            ../build/render $width $height $sample $output $depth
            end_time=$(date +%s.%N)
            execution_time=$(echo "$end_time - $start_time" | bc)
            echo "$width x $height,$execution_time" >> render_pixel_results.csv
        done
    done
done

echo "Tests completed. Results saved in render_pixel_results.csv"