#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<string.h>

#include <mpi.h>
#include <argp.h>

#include <jacobi-2d.h>

static int rank, tsteps, num_workers, chunk_size, start_row, end_row;

DECLARE_GRIDS
PREPARE_GRIDS

/* Worker function. Receives chunk from the root and calculate final values. */
static void
jacobi_2d_worker_mpi()
{
    /* Variable declaration. */
    int t, i, j, neigh_above, neigh_below;
    MPI_Status status;
    
    /* Get chunk limits for worker rank. */
    get_limits(rank, chunk_size, num_workers, &start_row, &end_row);

    neigh_above = rank - 1;
    neigh_below = rank == num_workers - 1 ? 0 : rank + 1;

    /* If it is the max rank, it must receive the bottom row. */
    if (!neigh_below && rank)
    {
        MPI_Recv(&INITIAL_GRID EL(N - 1, 0), N, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, &status);
        memcpy(&AUX_GRID EL(N - 1, 1), &INITIAL_GRID EL(N - 1, 1), sizeof(double) * (N - 2));
    }

    /* Receive initial chunk values from root. */
    MPI_Scatter(
            &INITIAL_GRID EL(start_row, 0), chunk_size * N, MPI_DOUBLE,
            &INITIAL_GRID EL(start_row, 0), chunk_size * N, MPI_DOUBLE,
            0, MPI_COMM_WORLD);

    for (i = start_row; i <= end_row; i++)
    {
        AUX_GRID EL(i, 0) = INITIAL_GRID EL(i, 0);
        AUX_GRID EL(i, N - 1) = INITIAL_GRID EL(i, N - 1);
    }

    /* Calculate chunk. */
    for (t = 0; t < tsteps; t++)
    {
        /* Share borders with neighbors above and below. */
        if (rank)
            MPI_Sendrecv(
                    &A EL(start_row, 0), N, MPI_DOUBLE, neigh_above, TAG,
                    &A EL(start_row - 1, 0), N, MPI_DOUBLE, neigh_above, TAG,
                    MPI_COMM_WORLD, &status);
        if (neigh_below)
            MPI_Sendrecv(
                    &A EL(end_row, 0), N, MPI_DOUBLE, neigh_below, TAG,
                    &A EL(end_row + 1, 0), N, MPI_DOUBLE, neigh_below, TAG,
                    MPI_COMM_WORLD, &status);

        if (rank == DEBUG_RANK && DEBUG)
        {
            printf("After sync %d of worker %d\n", t, rank);
            print_grid(A);
        }
        
        for (i = start_row; i <= end_row; i++)
	        for (j = 1; j < N - 1; j++)
	            B EL(i, j) = 0.2 * (A EL(i, j) + A EL(i, j - 1) + A EL(i, 1 + j) + A EL(1 + i, j) + A EL(i - 1, j));
    
        if (rank == DEBUG_RANK && DEBUG)
        {
            printf("Iter %d of worker %d\n", t, rank);
            print_grid(B);
        }

        SWAP_GRIDS
    }

    /* Send final chunk values from root. */
    MPI_Gather(
            &RESULT_GRID EL(start_row, 0), chunk_size * N, MPI_DOUBLE,
            &RESULT_GRID EL(start_row, 0), chunk_size * N, MPI_DOUBLE,
            0, MPI_COMM_WORLD);
}

/* Coordinator function. Initializes array, then share work between workers and also do work itself. */
static void
jacobi_2d_coordinator_mpi(int seed)
{
    /* Initialize array(s). */
    srand(seed);
    init_grid_with_copy(INITIAL_GRID, AUX_GRID);
        
    if (DEBUG)
        print_grid(INITIAL_GRID);

    /* Send bottom row to max rank worker .*/
    if (num_workers)
        MPI_Send(&INITIAL_GRID EL(N - 1, 0), N, MPI_DOUBLE, num_workers - 1, TAG, MPI_COMM_WORLD);

    jacobi_2d_worker_mpi(0);

    if (DEBUG)
        print_grid(RESULT_GRID);
}

/* The options we understand. */
struct argp_option options[] =
{
    { "size", 'd', "SIZE", 0, "Dataset size option (SMALL, MEDIUM, LARGE) - defines the number of iterations for the computation", 0 },
    { "seed", 's', "SEED", 0, "Seed for the array initialization", 0 },
    { 0 }
};

int
main(int argc, char *argv[])
{
    START_TIMER_MPI

    int seed = 1;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_workers);
    
    chunk_size = (N - 2) / num_workers;

    if (!rank)
    {
        struct arguments arguments;
        parse_args(argc, argv, &arguments);
        tsteps = arguments.size;
        seed = arguments.seed;
    }
    
    /* Syncronize parameters read by root. */
    MPI_Bcast(&tsteps, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&num_workers, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&chunk_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (!rank)
        jacobi_2d_coordinator_mpi(seed);
    else
        jacobi_2d_worker_mpi();


    MPI_Finalize();

    STOP_TIMER_MPI

    exit(EXIT_SUCCESS);
}
