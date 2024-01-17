/****************************************************************
 *                                                              *
 * This file has been written as a sample solution to an        *
 * exercise in a course given at the CSCS-USI Summer School.    *
 * It is made freely available with the understanding that      *
 * every copy of this file must include this header and that    *
 * CSCS/USI take no responsibility for the use of the enclosed  *
 * teaching material.                                           *
 *                                                              *
 * Purpose: : Parallel matrix-vector multiplication and the     *
 *            and power method                                  *
 *                                                              *
 * Contents: C-Source                                           *
 *                                                              *
 ****************************************************************/

#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <stdlib.h>
#include "hpc-power.h"

double power_method(double *partial_rows, double *vector, int matrix_size,
                    int rows_per_process, int rank, int n_iterations, MPI_Comm comm);
double dot_product(double *vectorA, double *vectorB, int length);
double vector_norm(double *vector, int length);
double *create_random_vector(int size);
void matrix_vector_multiply(double *partial_rows, double *vector, int matrix_size,
                            int rows_per_process, int rank, MPI_Comm comm);

int main(int argc, char *argv[])
{
    int my_rank, size; // size = number of processes
    int snd_buf, rcv_buf;
    int right, left;
    int sum, i;
    double start_time, elapsed_time;

    // Retrieve the number of iterations from command line
    char *p;
    int n_iterations = strtol(argv[2], &p, 10);

    MPI_Status status;
    MPI_Request request;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* This subproject is about to write a parallel program to
       multiply a matrix A by a vector x, and to use this routine in
       an implementation of the power method to find the absolute
       value of the largest eigenvalue of the matrix. Your code will
       call routines that we supply to generate matrices, record
       timings, and validate the answer.
    */

    // Remove for benchmark purposes: print the number of processes at the start of the program.
    if (my_rank == 0)
    {
        printf("Number of processes: %d\n", size);
    }

    // Convert command line argument to integer to determine matrix size.
    char *endptr;
    int matrix_size = strtol(argv[1], &endptr, 10);
    int rows_per_process = matrix_size / size;
    int start_row = rows_per_process * my_rank;

    // Create a vector and generate a matrix slice for each process.
    double *vector = create_random_vector(matrix_size);
    double *matrix_slice = hpc_generateMatrix(matrix_size, start_row, rows_per_process);

    // Uncomment to check vector x
    // if (my_rank == 0)
    //{
    //    for (int i = 0; i < matrix_size; ++i)
    //    {
    //        printf("%f\n", vector[i]);
    //    }
    //}

    // Start timer and perform power method to find largest eigenvalue.
    start_time = hpc_timer();
    double largest_eigenvalue = power_method(matrix_slice, vector, matrix_size, rows_per_process, my_rank, n_iterations, MPI_COMM_WORLD);
    elapsed_time = hpc_timer() - start_time;

    // Print results in the process with rank 0.
    if (my_rank == 0)
    {
        printf("Total time: %f\n", elapsed_time);
        for (int i = 0; i < matrix_size; ++i)
        {
            printf("%f\n", vector[i]);
        }
        printf("Verification: %d\n", hpc_verify(vector, matrix_size, elapsed_time));
        printf("Eigenvalue: %f\n", largest_eigenvalue);

        // For benchmarking purposes, print the execution time only:
        // printf("%f", elapsed_time);
    }

    MPI_Finalize();
    return 0;
}

/**
 * Implements the power method to estimate the largest eigenvalue of a matrix
 * in a parallel computing environment using MPI.
 *
 * The power method is iterated a fixed number of times (default is 1000).
 * In each iteration, the vector is normalized and then multiplied by the matrix.
 * The process is distributed across multiple MPI processes.
 *
 * @param partial_rows Pointer to the rows of the matrix assigned to this process.
 * @param vector The vector to be used in the power method.
 * @param matrix_size The size of the matrix (assuming a square matrix).
 * @param rows_per_process The number of rows of the matrix assigned to this process.
 * @param rank The rank of the current MPI process.
 * @param n_iterations The number of iterations of the matrix vector multiplication.
 * @param comm The MPI communicator.
 * @return The estimated largest eigenvalue of the matrix.
 */
double power_method(double *partial_rows, double *vector, int matrix_size,
                    int rows_per_process, int rank, int n_iterations, MPI_Comm comm)
{
    for (int i = 0; i < n_iterations; ++i)
    {
        if (rank == 0)
        {
            double norm_res = vector_norm(vector, matrix_size);

            for (int j = 0; j < matrix_size; ++j)
            {
                vector[j] /= norm_res;
            }
        }

        // Perform matrix-vector multiplication across all processes.
        matrix_vector_multiply(partial_rows, vector, matrix_size, rows_per_process, rank, comm);
    }

    // Return the norm of the vector, which approximates the largest eigenvalue.
    return vector_norm(vector, matrix_size);
}

/**
 * Calculates the dot product of two vectors.
 *
 * @param vectorA A pointer to the first input vector.
 * @param vectorB A pointer to the second input vector.
 * @param length The number of elements in each vector.
 * @return The dot product of the two vectors.
 */
double dot_product(double *vectorA, double *vectorB, int length)
{
    double sum = 0; // Initialize the sum of products to zero.

    // Iterate over each element in the vectors.
    for (int index = 0; index < length; ++index)
    {
        // Multiply corresponding elements and add to the sum.
        sum += vectorA[index] * vectorB[index];
    }

    return sum; // Return the computed dot product.
}

/**
 * Computes the L2 norm (Euclidean norm) of a vector.
 *
 * The L2 norm is calculated as the square root of the sum of the squares
 * of the vector's elements.
 *
 * @param vector Elements of the vector.
 * @param length The number of elements in the vector.
 * @return The L2 norm of the vector.
 */
double vector_norm(double *vector, int length)
{
    double sum_of_squares = 0;

    for (int index = 0; index < length; ++index)
    {
        sum_of_squares += vector[index] * vector[index];
    }

    return sqrt(sum_of_squares);
}

/**
 * Creates a vector of specified size with random elements normalized between 0 and 1.
 *
 * The function initializes a seed for random number generation to ensure replicability.
 * Each element of the vector is assigned a random value, which is then normalized
 * by dividing by the maximum value in the vector.
 *
 * @param size The size of the vector to be created.
 * @return Pointer to the created vector.
 */
double *create_random_vector(int size)
{
    double *vector = malloc(size * sizeof(double));

    srand(0);

    double max_value = 0.0;

    for (int i = 0; i < size; ++i)
    {
        vector[i] = (double)rand();
        if (vector[i] > max_value)
        {
            max_value = vector[i];
        }
    }

    for (int i = 0; i < size; ++i)
    {
        vector[i] /= max_value;
    }

    return vector;
}

/**
 * Performs matrix-vector multiplication in a parallel computing environment using MPI.
 *
 * This function broadcasts a vector 'x' to all processes, then each process computes
 * a portion of the resulting vector, and finally, the portions are gathered back.
 *
 * @param partial_rows Pointer to the rows of the matrix assigned to this process.
 * @param vector The vector to be multiplied with the matrix.
 * @param matrix_size The size of the matrix (assuming a square matrix).
 * @param rows_per_process The number of rows of the matrix assigned to this process.
 * @param rank The rank of the current MPI process.
 * @param comm The MPI communicator.
 */
void matrix_vector_multiply(double *partial_rows, double *vector, int matrix_size,
                            int rows_per_process, int rank, MPI_Comm comm)
{

    // Broadcast the vector to all processes.
    MPI_Bcast(vector, matrix_size, MPI_DOUBLE, 0, comm);

    double *temp_result = malloc(rows_per_process * sizeof(double));

    // Compute the multiplication for the assigned rows.
    for (int i = 0; i < rows_per_process; ++i)
    {
        double result = dot_product(partial_rows + (i * matrix_size), vector, matrix_size);
        temp_result[i] = result;
    }

    // Gather the partial results from all processes.
    MPI_Gather(temp_result, rows_per_process, MPI_DOUBLE, vector, rows_per_process, MPI_DOUBLE, 0, comm);

    free(temp_result);
}