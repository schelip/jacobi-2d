#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <mpi.h>
#include <pthread.h>
#include <argp.h>

#include <jacobi-2d.h>

THREAD_ARG_OPTIONS

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_barrier_t sync_barrier;
static int syncs_counter = 0;

static int rank, tsteps, num_workers, num_threads,
    chunk_size, strip_size, start_row, end_row;

DECLARE_GRIDS
PREPARE_GRIDS

/* Worker function. Calculates final elements in a certain strip of columns in a chunk of rows. */
static void *
jacobi_2d_worker_pthread(void* arg)
{
    int t, i, j, thread_id, start_col, end_col;

    thread_id = *((int*)arg);
    get_limits(thread_id, strip_size, num_threads, &start_col, &end_col);

    for (t = 0; t < tsteps; t++)
    {
        /* Wait for main thred to update borders. */
        pthread_mutex_lock(&mutex);
        if (syncs_counter <= t)
            pthread_cond_wait(&cond, &mutex);
        pthread_mutex_unlock(&mutex);

        /* Calculate one iteration of the strip. */
        for (i = start_row; i <= end_row; i++)
            for (j = start_col; j <= end_col; j++)
                B EL(i, j) = 0.2 * (A EL(i, j) + A EL(i, j - 1) + A EL(i, 1 + j) + A EL(1 + i, j) + A EL(i - 1, j));
        pthread_barrier_wait(&sync_barrier);

        if (thread_id == DEBUG_THREAD && rank == DEBUG_RANK && DEBUG)
        {
            printf("Iter %d\n", t);
            print_grid(B);
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

    /* Initialize threads and barriers. */
    threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
    if (threads == NULL)
    {
        fprintf(stderr, "Error allocating memory for threads");
        exit(EXIT_FAILURE);
    }

    pthread_barrier_init(&sync_barrier, NULL, num_threads + 1);

    /* Dispatch threads. */
    for (i = 0; i < num_threads; i++)
    {
        thread_id = (int*)malloc(sizeof(int));
        if (thread_id == NULL) {
            fprintf(stderr, "Error allocating memory for thread ID\n");
            exit(EXIT_FAILURE);
        }
        *thread_id = i;

        if (pthread_create(&threads[i], NULL, jacobi_2d_worker_pthread, (void*)thread_id) != 0)
        {
            fprintf(stderr, "Error creating thread %d", i);
            exit(EXIT_FAILURE);
        }
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

        /* Signal to threads that borders have been updated. */
        pthread_mutex_lock(&mutex);
        syncs_counter++;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);

        /* Wait for threads to finish calculating iteration and updating matrix. */
        pthread_barrier_wait(&sync_barrier);

        if (rank == DEBUG_RANK && DEBUG)
        {
            printf("Iter %d of worker %d\n", t, rank);
            print_grid(B);
        }

        SWAP_GRIDS
    }

    /* Wait for threads to finish and destroy barriers. */
    for (i = 0; i < num_threads; i++)
        if (pthread_join(threads[i], NULL) != 0)
        {
            fprintf(stderr, "Error joining thread %d", i);
            exit(EXIT_FAILURE);
        };
    
    pthread_barrier_destroy(&sync_barrier);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    /* Send final chunk values from root. */
    MPI_Gather(
            &RESULT_GRID EL(start_row, 0), chunk_size * N, MPI_DOUBLE,
            &RESULT_GRID EL(start_row, 0), chunk_size * N, MPI_DOUBLE,
            0, MPI_COMM_WORLD);
}

/* Coordinator function. Initializes array, then share work between workers and also do work itself. */
static void
jacobi_2d_coordinator_mpi()
{
    /* Initialize array(s). */
    init_grid_with_copy(INITIAL_GRID, AUX_GRID);
        
    if (DEBUG)
        print_grid(INITIAL_GRID);

    /* Send bottom row to max rank worker.*/
    if (num_workers)
        MPI_Send(&INITIAL_GRID EL(N - 1, 0), N, MPI_DOUBLE, num_workers - 1, TAG, MPI_COMM_WORLD);

    jacobi_2d_worker_mpi(0);

    if (DEBUG)
        print_grid(RESULT_GRID);
}

int
main(int argc, char *argv[])
{
    START_TIMER_MPI

    int provided;
    struct arguments arguments;

    MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &provided); 
    if(provided != MPI_THREAD_FUNNELED) 
    {
        fprintf(stderr, "This MPI implementation does not support MPI_THREAD_MULTIPLE.\n");
        exit(EXIT_FAILURE);
    }

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_workers);
    
    chunk_size = (N - 2) / num_workers;

    if (!rank)
    {
        parse_args(argc, argv, &arguments);
        tsteps = arguments.size;
        num_threads = arguments.threads;
        srand(arguments.seed);
        strip_size = (N - 2) / num_threads;
    }
    
    /* Syncronize parameters read by root. */
    MPI_Bcast(&tsteps, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&num_threads, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&strip_size, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (!rank)
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
