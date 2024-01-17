#!/bin/bash

# Create CSV file and write header
csv_file="scaling_strong_results.csv"
echo "Grid_Size,Num_Threads,Time_Spent" > $csv_file

# Grid sizes to be evaluated
grid_sizes=(64 128 256 512 1024)

# Number of threads from 1 to 24
for num_threads in {1..24}; do
    # Set the number of threads for OpenMP
    export OMP_NUM_THREADS=$num_threads

    # Iterate through each grid size
    for size in "${grid_sizes[@]}"; do
        # Run the program and redirect output to variable 'output'
        output=$(./main_benchmark $size 100 0.01)

        # Parse and write the results to the CSV file
        # Assuming the output format is "N;num_threads;time_spent;"
        IFS=';' read -ra ADDR <<< "$output"
        echo "${ADDR[0]},${ADDR[1]},${ADDR[2]}" >> $csv_file
    done
done
