#!/bin/bash

export OMP_PROC_BIND=spread
export OMP_PLACES=threads

INPUT_FILE="input_strong_scaling.txt"
OUTPUT="strong_scaling.txt"

echo "Test strong scaling..." > $OUTPUT

for t in 1 2 4 8 16
do
  export OMP_NUM_THREADS=$t
  echo "Running with $t threads..." >> $OUTPUT
  cat $INPUT_FILE | ../build/render 200 200 1 ../output/testMLT.ppm 5 1 >> $OUTPUT
  echo "--------------------------------------------------------------"
done