#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <pthread.h>
#include <argp.h>

#include <common.h>

// static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_barrier_t sync_barrier, calc_barrier;
static int rank, tsteps, num_workers, num_threads,
    chunk_size, strip_size, start_row, end_row;
static double A[N][N], B[N][N];

/* Calculates the first and last row indexes for the chunk of a given rank */
static void
get_limits(int rank, int chunk_size, int n_workers, int *start_row, int *end_row)
{
    *start_row = 1 + rank * chunk_size;
    *end_row = (rank == n_workers) ? (N - 2) : (*start_row + chunk_size) - 1;
}

/* Worker function. Calculates final elements in a certain strip of columns in a chunk of rows. */
static void *
jacobi_2d_worker_pthread(void* arg)
{
    int t, i, j, thread_id, start_col, end_col;

    thread_id = *((int*)arg);
    start_col = 1 + thread_id * strip_size;
    end_col = (thread_id == num_threads - 1) ? (N - 2) : (start_col + strip_size);

    for (t = 0; t < tsteps; t++)
    {
        /* Wait for main thred to update borders. */
        pthread_barrier_wait(&sync_barrier);

        /* Calculate one iteration of the strip. */
        for (i = start_row; i <= end_row; i++)
            for (j = start_col; j <= end_col; j++)
                B[i][j] = 0.2 * (A[i][j] + A[i][j - 1] + A[i][1 + j] + A[1 + i][j] + A[i - 1][j]);
        pthread_barrier_wait(&calc_barrier);

        /* Update matrix. */
        for (i = start_row; i <= end_row; i++)
            for (j = start_col; j <= end_col; j++)
                A[i][j] = B[i][j];
        pthread_barrier_wait(&sync_barrier);

        if (thread_id == DEBUG_THREAD && rank == DEBUG_RANK && DEBUG)
        {
            printf("Iter %d\n", t);
            print_array(A);
        }
    }

    return (void*)NULL;
}

/* Worker function. Receives chunk from the root and calculate final values. */
static void
jacobi_2d_worker_mpi()
{
    /* Variable declaration. */
    int t, i, *thread_id, neigh_above, neigh_below;
    pthread_t *threads;

    /* Get chunk limits for worker rank. */
    get_limits(rank, chunk_size, num_workers, &start_row, &end_row);

    neigh_above = rank - 1;
    neigh_below = rank == num_workers ? 0 : rank + 1;

    MPI_Status status;

    /* If it is the max rank, it must receive the bottom row. */
    if (!neigh_below)
        MPI_Recv(&A[N - 1][0], N, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, &status);

    /* Receive initial chunk values from root. */
    if (rank)
        for (i = start_row; i <= end_row; i++)
            MPI_Recv(&A[i][0], N, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, &status);

    /* Initialize threads and barriers. */
    threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
    if (threads == NULL)
    {
        fprintf(stderr, "Error allocating memory for threads");
        exit(1);
    }

    pthread_barrier_init(&sync_barrier, NULL, num_threads + 1);
    pthread_barrier_init(&calc_barrier, NULL, num_threads);

    /* Dispatch threads. */
    for (i = 0; i < num_threads; i++)
    {
        thread_id = (int*)malloc(sizeof(int));
        if (thread_id == NULL) {
            fprintf(stderr, "Error allocating memory for thread ID\n");
            exit(1);
        }
        *thread_id = i;

        if (pthread_create(&threads[i], NULL, jacobi_2d_worker_pthread, (void*)thread_id) != 0)
        {
            fprintf(stderr, "Error creating thread %d", i);
            exit(1);
        }
    }

    /* Calculate chunk. */
    for (t = 0; t < tsteps; t++)
    {
        /* Share borders with neighbors above and below. */
        if (rank)
            MPI_Send(&A[start_row][0], N, MPI_DOUBLE, neigh_above, TAG, MPI_COMM_WORLD);
        if (neigh_below)
        {
            MPI_Send(&A[end_row][0], N, MPI_DOUBLE, neigh_below, TAG, MPI_COMM_WORLD);
            MPI_Recv(&A[end_row + 1][0], N, MPI_DOUBLE, neigh_below, TAG, MPI_COMM_WORLD, &status);
        }
        if (rank)
            MPI_Recv(&A[start_row - 1][0], N, MPI_DOUBLE, neigh_above, TAG, MPI_COMM_WORLD, &status);

        /* Signal to threads that borders have been updated. */
        pthread_barrier_wait(&sync_barrier);

        /* Wait for threads to finish calculating iteration and updating matrix. */
        pthread_barrier_wait(&sync_barrier);
    }

    /* Wait for threads to finish and destroy barriers. */
    for (i = 0; i < num_threads; i++)
        if (pthread_join(threads[i], NULL) != 0)
        {
            fprintf(stderr, "Error joining thread %d", i);
            exit(1);
        };
    
    pthread_barrier_destroy(&sync_barrier);
    pthread_barrier_destroy(&calc_barrier);

    /* Send final chunk values from root. */
    if (rank)
        for (i = start_row; i <= end_row; i++)
            MPI_Send(&A[i][1], N - 1, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD);
}

/* Coordinator function. Initializes array, then share work between workers and also do work itself. */
static void
jacobi_2d_coordinator_mpi(int seed)
{
    /* Variable declaration. */
    int i, worker_start_row, worker_end_row, worker;

    /* Initialize array(s). */
    srand(seed);
    init_array(A);
        
    if (DEBUG)
        print_array(A);

    MPI_Status status;

    /* Send bottom row to max rank worker */
    MPI_Send(&A[N - 1][0], N, MPI_DOUBLE, num_workers, TAG, MPI_COMM_WORLD);

    /* Send initial values of chunks to workers */ 
    for (worker = 1; worker <= num_workers; worker++)
    {
        get_limits(worker, chunk_size, num_workers, &worker_start_row, &worker_end_row);
        for (i = worker_start_row; i <= worker_end_row; i++)
            MPI_Send(&A[i][0], N, MPI_DOUBLE, worker, TAG, MPI_COMM_WORLD);
    }

    jacobi_2d_worker_mpi(0);

    /* Receive final values of chunks from workers */
    for (worker = 1; worker <= num_workers; worker++)
    {
        get_limits(worker, chunk_size, num_workers, &worker_start_row, &worker_end_row);
        for (i = worker_start_row; i <= worker_end_row; i++)
            MPI_Recv(&A[i][1], N - 1, MPI_DOUBLE, worker, TAG, MPI_COMM_WORLD, &status);
    }

    if (DEBUG)
        print_array(A);
}

/* The options we understand. */
struct argp_option options[] =
{
    { "size", 'd', "SIZE", 0, "Dataset size option (SMALL, MEDIUM, LARGE) - defines the number of iterations for the computation", 0 },
    { "threads", 't', "THREADS", 0, "Number of threads for the parallel computation", 0 },
    { "seed", 's', "SEED", 0, "Seed for the array initialization", 0 },
    { 0 }
};

int
main(int argc, char *argv[])
{
    int seed = 1, provided;

    MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &provided); 
    if(provided != MPI_THREAD_FUNNELED) 
    {
        fprintf(stderr, "This MPI implementation does not support MPI_THREAD_MULTIPLE.\n");
        exit(1);
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_workers);
    
    chunk_size = (N - 2) / num_workers;
    num_workers--;

    if (!rank)
    {
        struct arguments arguments;
        parse_args(argc, argv, &arguments);
        tsteps = arguments.size;
        seed = arguments.seed;
        num_threads = arguments.threads;
        strip_size = (N - 2) / num_threads;
    }
    
    /* Syncronize parameters read by root. */
    MPI_Bcast(&tsteps, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&num_threads, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&strip_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (!rank)
        jacobi_2d_coordinator_mpi(seed);
    else
        jacobi_2d_worker_mpi();

    MPI_Finalize();
    exit(0);
}
