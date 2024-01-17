/****************************************************************
 *                                                              *
 * This file has been written as a sample solution to an        *
 * exercise in a course given at the CSCS-USI Summer School     *
 * It is made freely available with the understanding that      *
 * every copy of this file must include this header and that    *
 * CSCS/USI take no responsibility for the use of the enclosed  *
 * teaching material.                                           *
 *                                                              *
 * Purpose: Exchange ghost cell in 2 directions using a topology*
 *                                                              *
 * Contents: C-Source                                           *
 *                                                              *
 ****************************************************************/

/* Use only 16 processes for this exercise
 * Send the ghost cell in two directions: left<->right and top<->bottom
 * ranks are connected in a cyclic manner, for instance, rank 0 and 12 are connected
 *
 * process decomposition on 4*4 grid
 *
 * |-----------|
 * | 0| 1| 2| 3|
 * |-----------|
 * | 4| 5| 6| 7|
 * |-----------|
 * | 8| 9|10|11|
 * |-----------|
 * |12|13|14|15|
 * |-----------|
 *
 * Each process works on a 6*6 (SUBDOMAIN) block of data
 * the D corresponds to data, g corresponds to "ghost cells"
 * xggggggggggx
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * gDDDDDDDDDDg
 * xggggggggggx
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SUBDOMAIN 6
#define DOMAINSIZE (SUBDOMAIN + 2)

enum direction
{
    TO_TOP,
    TO_BOTTOM,
    TO_LEFT,
    TO_RIGHT
};

int main(int argc, char *argv[])
{
    int rank, size, i, j, dims[2], periods[2], rank_top, rank_bottom, rank_left, rank_right;
    double data[DOMAINSIZE * DOMAINSIZE];
    MPI_Request request;
    MPI_Status status;
    MPI_Comm comm_cart;
    MPI_Datatype ghost_row;
    MPI_Datatype ghost_col;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 16)
    {
        printf("please run this with 16 processors\n");
        MPI_Finalize();
        exit(1);
    }

    // Initialize the domain with the current rank value
    for (i = 0; i < DOMAINSIZE * DOMAINSIZE; i++)
    {
        data[i] = rank;
    }

    // Set the dimensions of the processor grid and periodic boundaries in both dimensions
    dims[0] = 4;
    dims[1] = 4;
    periods[0] = 1; // true
    periods[1] = 1; // true

    // Create a Cartesian communicator (4*4) with periodic boundaries (we do not allow
    // the reordering of ranks) and use it to find your neighboring
    // ranks in all dimensions in a cyclic manner.
    int reorder = 0; // false
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &comm_cart);

    // Find your top/bottom/left/right neighbor using the new communicator, see MPI_Cart_shift()
    // rank_left, rank_right
    MPI_Cart_shift(comm_cart, 1, 1, &rank_left, &rank_right);
    // rank_top, rank_bottom
    MPI_Cart_shift(comm_cart, 0, 1, &rank_top, &rank_bottom);

    //  Create derived datatype data_ghost, create a datatype for sending the column, see MPI_Type_vector() and MPI_Type_commit()
    // data_ghost
    MPI_Type_vector(6, 1, 1, MPI_DOUBLE, &ghost_row); // Row
    MPI_Type_commit(&ghost_row);

    MPI_Type_vector(6, 1, 8, MPI_DOUBLE, &ghost_col); // Column
    MPI_Type_commit(&ghost_col);

    //  Ghost cell exchange with the neighboring cells in all directions
    //  use MPI_Irecv(), MPI_Send(), MPI_Wait() or other viable alternatives

    //  to the top
    MPI_Irecv(&data[DOMAINSIZE * DOMAINSIZE - DOMAINSIZE + 1], 1, ghost_row, rank_bottom, TO_TOP, comm_cart, &request);
    MPI_Send(&data[DOMAINSIZE + 1], 1, ghost_row, rank_top, TO_TOP, comm_cart);
    MPI_Wait(&request, &status);

    //  to the bottom
    MPI_Irecv(&data[1], 1, ghost_row, rank_top, TO_BOTTOM, comm_cart, &request);
    MPI_Send(&data[DOMAINSIZE * DOMAINSIZE - 2 * DOMAINSIZE + 1], 1, ghost_row, rank_bottom, TO_BOTTOM, comm_cart);
    MPI_Wait(&request, &status);

    //  to the left
    MPI_Irecv(&data[2 * DOMAINSIZE - 1], 1, ghost_col, rank_right, TO_LEFT, comm_cart, &request);
    MPI_Send(&data[DOMAINSIZE + 1], 1, ghost_col, rank_left, TO_LEFT, comm_cart);
    MPI_Wait(&request, &status);

    //  to the right
    MPI_Irecv(&data[DOMAINSIZE], 1, ghost_col, rank_left, TO_RIGHT, comm_cart, &request);
    MPI_Send(&data[DOMAINSIZE + 6], 1, ghost_col, rank_right, TO_RIGHT, comm_cart);
    MPI_Wait(&request, &status);

    if (rank == 9)
    {
        printf("data of rank 9 after communication\n");
        for (j = 0; j < DOMAINSIZE; j++)
        {
            for (i = 0; i < DOMAINSIZE; i++)
            {
                printf("%.1f ", data[i + j * DOMAINSIZE]);
            }
            printf("\n");
        }
    }

    // Uncomment when done with tasks above
    MPI_Type_free(&ghost_row);
    MPI_Type_free(&ghost_col);
    MPI_Comm_free(&comm_cart);
    MPI_Finalize();

    return 0;
}
