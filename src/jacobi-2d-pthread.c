#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <argp.h>

#include <common.h>

/* Shared variables between all threads */
static pthread_barrier_t barrier;
static int tsteps, num_threads, chunk_size;
static double A[N][N], B[N][N];
    
static void *
jacobi_2d_worker_pthread(void* arg)
{
    int t, i, j, thread_id, start_row, end_row;

    thread_id = *((int*)arg);
    start_row = 1 + thread_id * chunk_size;
    end_row = (thread_id == num_threads - 1) ? (N - 1) : (start_row + chunk_size);

    for (t = 0; t < tsteps; t++)
    {
        for (i = start_row; i < end_row; i++)
            for (j = 1; j < N - 1; j++)
                B[i][j] = 0.2 * (A[i][j] + A[i][j - 1] + A[i][1 + j] + A[1 + i][j] + A[i - 1][j]);
        pthread_barrier_wait(&barrier);

        for (i = start_row; i < end_row; i++)
            for (j = 1; j < N - 1; j++)
                A[i][j] = B[i][j];
        pthread_barrier_wait(&barrier);

        if (thread_id == 0 && DEBUG)
        {
            printf("Iter %d\n", t);
            print_array(A);
        }
    }

    return (void*)NULL;
}

static void
jacobi_2d_pthread(int seed) {
    int i, *thread_id;
    pthread_t *threads;

    chunk_size = (N - 2) / num_threads;

    /* Initialize array(s). */
    srand(seed);
    init_array_with_copy(A, B);

    /* Initialize threads and barrier. */
    threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
    if (threads == NULL)
    {
        fprintf(stderr, "Error allocating memory for threads");
        exit(1);
    }

    pthread_barrier_init(&barrier, NULL, num_threads);

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

    /* Wait for threads to finish and destroy bxarrier. */
    for (i = 0; i < num_threads; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            fprintf(stderr, "Error joining thread %d", i);
            exit(1);
        };
    }
    
    pthread_barrier_destroy(&barrier);

    if (DEBUG) print_array(A);
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
    struct arguments arguments;
    parse_args(argc, argv, &arguments);
    tsteps = arguments.size;
    num_threads = arguments.threads;
    jacobi_2d_pthread(arguments.seed);
    exit(0);
}
