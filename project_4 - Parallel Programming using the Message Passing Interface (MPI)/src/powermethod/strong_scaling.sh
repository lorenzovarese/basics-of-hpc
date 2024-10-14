#!/bin/bash

# Define the matrix size for strong scaling (fixed matrix size)
MATRIX_SIZE=10000
ITERATIONS=1000
OUTPUT_STRONG=strong_scaling.csv

echo "Start script - Strong scaling"

# Header for CSV file
echo "processes,execution_time,efficiency" > $OUTPUT_STRONG

echo "Start Serial Time..."

# Run the serial version
SERIAL_TIME=$(mpirun -np 1 ./powermethod $MATRIX_SIZE $ITERATIONS)
echo "1,$SERIAL_TIME,1" >> $OUTPUT_STRONG

for PROCESSES in 2 4 8 16 32 64; do
    echo "Start MPI benchmark with $PROCESSES size..."
    # Run the parallel version
    EXEC_TIME=$(mpirun -np $PROCESSES --oversubscribe ./powermethod $MATRIX_SIZE $ITERATIONS)
    
    # Calculate efficiency
    EFFICIENCY=$(echo "$SERIAL_TIME / ($EXEC_TIME * $PROCESSES)" | bc -l)
    
    # Append to CSV
    echo "$PROCESSES,$EXEC_TIME,$EFFICIENCY" >> $OUTPUT_STRONG
done

echo "Done."
