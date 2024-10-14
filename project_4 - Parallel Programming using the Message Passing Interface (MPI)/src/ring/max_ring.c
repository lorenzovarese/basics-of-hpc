/****************************************************************
 *                                                              *
 * This file has been written as a sample solution to an        *
 * exercise in a course given at the CSCS-USI Summer School.    *
 * It is made freely available with the understanding that      *
 * every copy of this file must include this header and that    *
 * CSCS/USI take no responsibility for the use of the enclosed  *
 * teaching material.                                           *
 *                                                              *
 * Purpose: Parallel maximum using a ping-pong                  *
 *                                                              *
 * Contents: C-Source                                           *
 *                                                              *
 ****************************************************************/

#include <stdio.h>
#include <mpi.h>
#include <string.h>

int ring_sum_algorithm(int rank, int size, int left, int right, void *status)
{
    int snd_buf = rank;
    int rcv_buf = 0;
    int max = 0;

    for (int i = 0; i < size; i++)
    {
        MPI_Sendrecv(&snd_buf, 1, MPI_INT, right, 0,
                     &rcv_buf, 1, MPI_INT, left, 0,
                     MPI_COMM_WORLD, status);
        max += rcv_buf;
        snd_buf = rcv_buf;
    }

    return max;
}

int custom_ring_algorithm(int rank, int size, int left, int right, void *status)
{
    int snd_buf = (3 * rank) % (2 * size);
    int max = snd_buf;
    int rcv_buf = 0;

    for (int i = 0; i < size; i++)
    {
        MPI_Sendrecv(&snd_buf, 1, MPI_INT, right, 0,
                     &rcv_buf, 1, MPI_INT, left, 0,
                     MPI_COMM_WORLD, status);

        if (rcv_buf > max)
            max = rcv_buf;

        snd_buf = max;
    }

    return max;
}

int main(int argc, char *argv[])
{
    int my_rank, size;
    int snd_buf, rcv_buf;
    int right, left;
    int max, max_ring_algo, i;
    int local_stack[size];

    MPI_Status status;
    MPI_Request request;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // [1] Determines the left/right neighbours of each process
    right = (my_rank + 1) % size;
    left = (my_rank - 1 + size) % size;

    // [2] Global maximum computation
    max = custom_ring_algorithm(my_rank, size, left, right, &status);
    printf("Process %i:\tMax = %i\n", my_rank, max);

    // [3] Basic algorithm
    // MPI_Barrier(MPI_COMM_WORLD);
    // max_ring_algo = ring_sum_algorithm(my_rank, size, left, right, &status);
    // printf("(Basic) Process %i:\tMax = %i\n", my_rank, max_ring_algo);

    MPI_Finalize();
    return 0;
}
