#!/bin/bash

# Define the sizes, threads, and ranks arrays
sizes=(128 256 512 1024)
threads=(1 2 3 4 5 6 7 8 9 10)
ranks=(1 2 4)

# Output CSV file name
output_file="performance_results.csv"

# Create or clear the output file at the start
echo "Size,Rank,Thread,Execution Time" > "$output_file"

# Iterate over each size, rank, and thread combination
for size in "${sizes[@]}"; do
    for rank in "${ranks[@]}"; do
        for thread in "${threads[@]}"; do
            export OMP_NUM_THREADS=$thread
            echo "Running with ranks=${rank}, threads=${thread}, size=${size}"

            # Run the program and append the results to the CSV file
            mpirun -np $rank ./main $size $size 100 0.01 | awk -F';' 'BEGIN{OFS=","} {print $1, $2, $3, $4}' >> "$output_file"
        done
    done
done

echo "Script execution completed. Results saved in $output_file."
