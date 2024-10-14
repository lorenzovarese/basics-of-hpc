#!/bin/bash

# Base matrix size for one process
BASE_MATRIX_SIZE=4096
ITERATIONS=100
OUTPUT_WEAK=weak_scaling.csv
BASE_TIME=0

echo "Start script - Weak scaling"

# Header for CSV file
echo "processes,matrix_size,execution_time,efficiency" > $OUTPUT_WEAK

echo "Start MPI benchmark with size=1..."
PROCESSES=1
MATRIX_SIZE=$(($BASE_MATRIX_SIZE))
BASE_TIME=$(mpirun -np $PROCESSES --oversubscribe ./powermethod $MATRIX_SIZE $ITERATIONS)
EXEC_TIME=$BASE_TIME
EFFICIENCY=$(echo "$BASE_TIME / $EXEC_TIME" | bc -l)
echo "$PROCESSES,$MATRIX_SIZE,$EXEC_TIME,$EFFICIENCY" >> $OUTPUT_WEAK

echo "Start MPI benchmark with size=2..."
PROCESSES=2
MATRIX_SIZE=5792
EXEC_TIME=$(mpirun -np $PROCESSES --oversubscribe ./powermethod $MATRIX_SIZE $ITERATIONS)
EFFICIENCY=$(echo "$BASE_TIME / $EXEC_TIME" | bc -l)
echo "$PROCESSES,$MATRIX_SIZE,$EXEC_TIME,$EFFICIENCY" >> $OUTPUT_WEAK

echo "Start MPI benchmark with size=4..."
PROCESSES=4
MATRIX_SIZE=$(($BASE_MATRIX_SIZE * 2))
EXEC_TIME=$(mpirun -np $PROCESSES --oversubscribe ./powermethod $MATRIX_SIZE $ITERATIONS)
EFFICIENCY=$(echo "$BASE_TIME / $EXEC_TIME" | bc -l)
echo "$PROCESSES,$MATRIX_SIZE,$EXEC_TIME,$EFFICIENCY" >> $OUTPUT_WEAK

echo "Start MPI benchmark with size=8..."
PROCESSES=8
MATRIX_SIZE=11552
EXEC_TIME=$(mpirun -np $PROCESSES --oversubscribe ./powermethod $MATRIX_SIZE $ITERATIONS)
EFFICIENCY=$(echo "$BASE_TIME / $EXEC_TIME" | bc -l)
echo "$PROCESSES,$MATRIX_SIZE,$EXEC_TIME,$EFFICIENCY" >> $OUTPUT_WEAK

echo "Start MPI benchmark with size=16..."
PROCESSES=16
MATRIX_SIZE=$(($BASE_MATRIX_SIZE * 4))
EXEC_TIME=$(mpirun -np $PROCESSES --oversubscribe ./powermethod $MATRIX_SIZE $ITERATIONS)
EFFICIENCY=$(echo "$BASE_TIME / $EXEC_TIME" | bc -l)
echo "$PROCESSES,$MATRIX_SIZE,$EXEC_TIME,$EFFICIENCY" >> $OUTPUT_WEAK

echo "Start MPI benchmark with size=32..."
MATRIX_SIZE=23168
PROCESSES=32
EXEC_TIME=$(mpirun -np $PROCESSES --oversubscribe ./powermethod $MATRIX_SIZE $ITERATIONS)
EFFICIENCY=$(echo "$BASE_TIME / $EXEC_TIME" | bc -l)
echo "$PROCESSES,$MATRIX_SIZE,$EXEC_TIME,$EFFICIENCY" >> $OUTPUT_WEAK

echo "Start MPI benchmark with size=64..."
MATRIX_SIZE=$(($BASE_MATRIX_SIZE * 8))
PROCESSES=64
EXEC_TIME=$(mpirun -np $PROCESSES --oversubscribe ./powermethod $MATRIX_SIZE $ITERATIONS)
EFFICIENCY=$(echo "$BASE_TIME / $EXEC_TIME" | bc -l)
echo "$PROCESSES,$MATRIX_SIZE,$EXEC_TIME,$EFFICIENCY" >> $OUTPUT_WEAK

echo "Done."
