#!/bin/bash

run_benchmark() {
  local threads=$1
  local grid_size=$2
  local nt=$3
  local t=$4
  local csv_file=$5

  # Set number of threads and run benchmark
  export OMP_NUM_THREADS=$threads
  output=$(./main_benchmark $grid_size $nt $t)
  
  # Parse and append the output to CSV file
  IFS=';' read -ra ADDR <<< "$output"
  echo "${ADDR[0]},${ADDR[1]},${ADDR[2]}" >> $csv_file
}

# Create CSV file and write header
csv_file="scaling_weak_results.csv"
echo "Grid_Size,Num_Threads,Time_Spent" > $csv_file

# Weak comparison: 1 thread 128 vs 4 thread 256
run_benchmark 1 128 100 0.005 $csv_file
run_benchmark 4 256 100 0.005 $csv_file

# Weak comparison: 2 thread 256 vs 8 thread 512
run_benchmark 2 256 100 0.005 $csv_file
run_benchmark 8 512 100 0.005 $csv_file

# Weak comparison: 2 thread 512 vs 8 thread 1024
run_benchmark 2 512 100 0.005 $csv_file
run_benchmark 8 1024 100 0.005 $csv_file

# Weak comparison: 2 thread 1024 vs 8 thread 2048
run_benchmark 2 1024 100 0.001 $csv_file
run_benchmark 8 2048 100 0.001 $csv_file
