#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <argp.h>

#include <polybench.h>

#define DEBUG_RANK -1

/* Calculates the first and last row indexes for the chunk of a given rank */
static void
get_limits(int rank, int chunk_size, int n_workers, int *start_row, int *end_row)
{
    *start_row = 1 + rank * chunk_size;
    *end_row = (rank == n_workers) ? (N - 2) : (*start_row + chunk_size) - 1;
}

/* Worker function. Receives chunk from the root and calculate final values. */
static void
jacobi_2d_worker_mpi(int rank, int n_workers, int chunk_size)
{
    /* Variable declaration. */
    int t, tsteps, i, j, neigh_above, neigh_below, start_row, end_row;
    double A[N][N], B[N][N];

    /* Get chunk limits for worker rank. */
    get_limits(rank, chunk_size, n_workers, &start_row, &end_row);

    neigh_above = rank - 1;
    neigh_below = (rank == n_workers) ? 0 : rank + 1;

    MPI_Status status;

    /* Receive number of iterations from root. */
    MPI_Bcast(&tsteps, 1, MPI_INT, 0, MPI_COMM_WORLD);

    /* If it is the max rank, it must receive the bottom row. */
    if (!neigh_below) {
        MPI_Recv(&A[N - 1][0], N, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, &status);
    }

    /* Receive initial chunk values from root. */
    for (i = start_row; i <= end_row; i++)
        MPI_Recv(&A[i][0], N, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, &status);

    /* Calculate chunk. */
    for (t = 0; t < tsteps; t++)
    {
        /* Share borders with neighbors above and below. */
        MPI_Send(&A[start_row][0], N, MPI_DOUBLE, neigh_above, TAG, MPI_COMM_WORLD);
        if (neigh_below)
        {
            MPI_Send(&A[end_row][0], N, MPI_DOUBLE, neigh_below, TAG, MPI_COMM_WORLD);
            MPI_Recv(&A[end_row + 1][0], N, MPI_DOUBLE, neigh_below, TAG, MPI_COMM_WORLD, &status);
        }
        MPI_Recv(&A[start_row - 1][0], N, MPI_DOUBLE, neigh_above, TAG, MPI_COMM_WORLD, &status);
        
        for (i = start_row; i <= end_row; i++)
	        for (j = 1; j < N - 1; j++)
	            B[i][j] = 0.2 * (A[i][j] + A[i][j - 1] + A[i][1 + j] + A[1 + i][j] + A[i - 1][j]);
        for (i = start_row; i <= end_row; i++)
	        for (j = 1; j < N - 1; j++)
	            A[i][j] = B[i][j];
    
        if (rank == DEBUG_RANK && DEBUG)
        {
            printf("Iter %d of worker %d\n", t, rank);
            print_array(A);
        }
    }

    /* Send final chunk values from root. */
    for (i = start_row; i <= end_row; i++)
        MPI_Send(&A[i][1], N - 1, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD);
}

/* Coordinator function. Initializes array, then share work between workers and also do work itself. */
static void
jacobi_2d_coordinator_mpi(int tsteps, int seed, int n_workers, int chunk_size)
{
    /* Variable declaration. */
    int t, i, j, start_row, end_row, worker;
    double A[N][N], B[N][N];

    /* Initialize array(s). */
    srand(seed);
    init_array(A);
        
    if (DEBUG)
        print_array(A);

    MPI_Status status;

    /* Broadcast number of iterations to workers */
    MPI_Bcast(&tsteps, 1, MPI_INT, 0, MPI_COMM_WORLD);

    /* Send bottom row to max rank worker */
    MPI_Send(&A[N - 1][0], N, MPI_DOUBLE, n_workers, TAG, MPI_COMM_WORLD);

    /* Send initial values of chunks to workers */ 
    for (worker = 1; worker <= n_workers; worker++)
    {
        get_limits(worker, chunk_size, n_workers, &start_row, &end_row);
        for (i = start_row; i <= end_row; i++)
            MPI_Send(&A[i][0], N, MPI_DOUBLE, worker, TAG, MPI_COMM_WORLD);
    }

    /* Calculate first chunk. */
    start_row = 1;
    end_row = chunk_size;
    for (t = 0; t < tsteps; t++)
    {
        /* Share borders with neighbor below. */
        MPI_Send(&A[end_row][0], N, MPI_DOUBLE, 1, TAG, MPI_COMM_WORLD);
        MPI_Recv(&A[end_row + 1][0], N, MPI_DOUBLE, 1, TAG, MPI_COMM_WORLD, &status);

        for (i = 1; i <= end_row; i++)
	        for (j = 1; j < N - 1; j++)
	            B[i][j] = 0.2 * (A[i][j] + A[i][j - 1] + A[i][1 + j] + A[1 + i][j] + A[i - 1][j]);
        for (i = 1; i <= end_row; i++)
	        for (j = 1; j < N - 1; j++)
	            A[i][j] = B[i][j];
    
        if (DEBUG_RANK == 0 && DEBUG)
        {
            printf("Iter %d of worker 0\n", t);
            print_array(A);
        }
    }

    /* Receive final values of chunks from workers */
    for (worker = 1; worker <= n_workers; worker++)
    {
        get_limits(worker, chunk_size, n_workers, &start_row, &end_row);
        for (i = start_row; i <= end_row; i++) {
            MPI_Recv(&A[i][1], N - 1, MPI_DOUBLE, worker, TAG, MPI_COMM_WORLD, &status);
        }
    }

    if (DEBUG)
        print_array(A);
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
    int rank, n_workers, chunk_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_workers);
    
    chunk_size = (N - 2) / n_workers;
    n_workers--;

    if (rank == 0) {
        struct arguments arguments;
        parse_args(argc, argv, &arguments);
        jacobi_2d_coordinator_mpi(arguments.size, arguments.seed, n_workers, chunk_size);
    } else {
        jacobi_2d_worker_mpi(rank, n_workers, chunk_size);
    }

    MPI_Finalize();
    exit(0);
}
