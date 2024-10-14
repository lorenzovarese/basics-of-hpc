#!/bin/bash

# Output CSV file
output_file="performanceData.csv"

# Initialize CSV file with headers
echo "N,numTh-Method,ExecutionTime" > $output_file

# Vector lengths
N_values=("100000" "1000000" "10000000" "100000000")

# Number of threads
thread_counts=("1" "2" "4" "8" "12" "16" "20" "24")

echo "### Run the serial version ###"

# Run the serial version
for N in "${N_values[@]}"; do
  
  echo "Run the serial version - N=${N}"

  # Record the execution time
  exec_time=$(./dotProductSerial $N)
  echo "$N,1T-Serial,$exec_time" >> $output_file
done

echo
echo "### Run the parallel version ###"

# Run the parallel versions
export OMP_NUM_THREADS=1 # This sets the environment variable for OpenMP

for N in "${N_values[@]}"; do
  for p in "${thread_counts[@]}"; do
    export OMP_NUM_THREADS=$p

    echo "Run the ${p}T-ParallelReduction version N=${N}"
    
    # Run your program and record the execution time for the reduction version
    exec_time_red=$(./dotProductReduction $N)
    echo "$N,${p}T-ParallelReduction,$exec_time_red" >> $output_file
    sleep 3

    echo "Run the ${p}T-CriticalReduction version N=${N}"
    
    # Run your program and record the execution time for the critical version
    exec_time_critical=$(./dotProductCritical $N)
    echo "$N,${p}T-ParallelCritical,$exec_time_critical" >> $output_file
    sleep 3
  done
done

echo "### Output saved in ${output_file} ###"