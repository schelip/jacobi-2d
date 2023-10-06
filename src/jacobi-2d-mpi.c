#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<string.h>

#include <mpi.h>
#include <argp.h>

#include <jacobi-2d.h>

ARG_OPTIONS

static int rank, tsteps, num_workers, chunk_size,
    start_row, end_row, extra_rows;
static MPI_Status status;

DECLARE_GRIDS
PREPARE_GRIDS

/* Worker function. Receives chunk from the root and calculate final values. */
static void
jacobi_2d_worker_mpi()
{
    /* Variable declaration. */
    int t, i, j, neigh_above, neigh_below;
    
    /* Get chunk limits for worker rank. */
    get_limits(rank, chunk_size, num_workers, &start_row, &end_row);

    neigh_above = rank - 1;
    neigh_below = rank == num_workers - 1 ? 0 : rank + 1;

    /* If it is the max rank, it must receive the extra rows and the bottom external row. */
    if (!neigh_below && rank)
    {
        MPI_Recv(&INITIAL_GRID EL(N - extra_rows, 0), N * extra_rows, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, &status);
        memcpy(&AUX_GRID EL(N - extra_rows, 0), &INITIAL_GRID EL(N - extra_rows, 0), sizeof(double) * N * extra_rows);
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

    /* If it is the max rank, it must send the extra rows. */
    if (!neigh_below && rank)
        MPI_Send(&RESULT_GRID EL(N - extra_rows, 0), N * (extra_rows - 1), MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD);
}

/* Coordinator function. Initializes array, then share work between workers and also do work itself. */
static void
jacobi_2d_coordinator_mpi()
{
    /* Initialize array(s). */
    init_grid_with_copy(INITIAL_GRID, AUX_GRID);
        
    if (DEBUG)
        print_grid(INITIAL_GRID);

    /* Send extra rows and bottom row to max rank worker .*/
    if (num_workers)
        MPI_Send(&INITIAL_GRID EL(N - extra_rows, 0), N * extra_rows, MPI_DOUBLE, num_workers - 1, TAG, MPI_COMM_WORLD);

    jacobi_2d_worker_mpi(0);

    /* Receive extra rows from max rank worker .*/
    if (num_workers)
        MPI_Recv(&RESULT_GRID EL(N - extra_rows, 0), N * (extra_rows - 1), MPI_DOUBLE, num_workers - 1, TAG, MPI_COMM_WORLD, &status);

    if (DEBUG)
        print_grid(RESULT_GRID);
}

int
main(int argc, char *argv[])
{
    START_TIMER_MPI

    struct arguments arguments;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_workers);
    
    chunk_size = (N - 2) / num_workers;
    extra_rows = ((N - 2) % num_workers) + 1;

    if (rank == 0)
    {
        parse_args(argc, argv, &arguments);
        tsteps = arguments.size;
        srand(arguments.seed);
    }

    VERIFY_NUM_PROCESSES
    
    /* Syncronize parameters read by root. */
    MPI_Bcast(&tsteps, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&num_workers, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&chunk_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
        jacobi_2d_coordinator_mpi();
    else
        jacobi_2d_worker_mpi();


    MPI_Finalize();

    STOP_TIMER_MPI
    
    if (rank == 0)
    {
        if (arguments.print_result)
            print_grid(RESULT_GRID);
        else
            PRINT_EXEC_TIME
    }

    exit(EXIT_SUCCESS);
}
