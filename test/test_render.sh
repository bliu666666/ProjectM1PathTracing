#!/bin/bash

# Parameter settings
resolutions=("200x100" "400x200" "800x400" "1600x800")
samples=("10" "50" "100" "1000")
depths=("10" "50")

# Create CSV file with headers
echo "Resolution,Samples,Depth,Time(s)" > render_results.csv

# Iterate over parameter combinations
for res in "${resolutions[@]}"; do
    for sample in "${samples[@]}"; do
        for depth in "${depths[@]}"; do
            width=$(echo $res | cut -d'x' -f1)
            height=$(echo $res | cut -d'x' -f2)
            output="output_${width}x${height}_${sample}_${depth}.ppm"
            
            echo "Running render with resolution=${width}x${height}, samples=${sample}, depth=${depth}"
            
            # Measure execution time
            start_time=$(date +%s.%N)
            ../build/render $width $height $sample $output $depth
            end_time=$(date +%s.%N)
            execution_time=$(echo "$end_time - $start_time" | bc)
            
            # Save results to CSV
            echo "$width x $height,$sample,$depth,$execution_time" >> render_results.csv
            
            # Clean up output image
            rm $output
        done
    done
done

echo "Tests completed. Results saved in render_results.csv"
