#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#include <polybench.h>

pthread_barrier_t barrier;

static int tsteps, num_threads, chunk_size;
static DATA_TYPE POLYBENCH_2D(POLYBENCH_DECL_VAR(A), N, N);
static DATA_TYPE POLYBENCH_2D(POLYBENCH_DECL_VAR(B), N, N);

    
void* kernel_jacobi_2d_parallel(void* arg) {
    int t, i, j;
    int thread_id = *((int*)arg);

    DATA_TYPE (*a)[N] = POLYBENCH_ARRAY(A);
    DATA_TYPE (*b)[N] = POLYBENCH_ARRAY(B);

    if (DEBUG) fprintf(stdout, "[Thread %d] Created\n", thread_id);

    int start_row = 1 + thread_id * chunk_size;
    int end_row = (thread_id == num_threads - 1) ? (N - 1) : (start_row + chunk_size);

    if (DEBUG) fprintf(stdout, "[Thread %d] Start: %d - End: %d\n", thread_id, start_row, end_row - 1);

    for (t = 0; t < tsteps; t++) {
        if (DEBUG) fprintf(stdout, "[Thread %d] Started iteration %d\n", thread_id, t);
        for (i = start_row; i < end_row; i++)
            for (j = 1; j < N - 1; j++) {
                b[i][j] = 0.2 * (a[i][j] + a[i][j - 1] + a[i][1 + j] + a[1 + i][j] + a[i - 1][j]);
                // if (DEBUG) printf("[Thread %d] Old: %f New: %f\n", thread_id, a[i][j], b[i][j]);
            }

        if (DEBUG) fprintf(stdout, "[Thread %d] Finished computation\n", thread_id);
        pthread_barrier_wait(&barrier);
        if (DEBUG && thread_id == 0) fprintf(stdout, "(Iteration %d) All threads finished computation\n", t);
        pthread_barrier_wait(&barrier);

        for (i = start_row; i < end_row; i++)
            for (j = 1; j < N - 1; j++)
                a[i][j] = b[i][j];
        if (DEBUG) fprintf(stdout, "[Thread %d] Finished update\n", thread_id);
        pthread_barrier_wait(&barrier);
        if (DEBUG && thread_id == 0) fprintf(stdout, "(Iteration %d) All threads finished update\n", t);
        pthread_barrier_wait(&barrier);

        if (DEBUG) {
            if (thread_id == 0) print_array(N, *A);
            pthread_barrier_wait(&barrier);
        }
    }

    return (void*)NULL;
}

void jacobi_2d_parallel(int t, int dce, int n_threads, int seed) {
    int i;

    tsteps = t;
    num_threads = n_threads;
    chunk_size = (N - 2) / num_threads;

    if (DEBUG)
        fprintf(stdout, "Matrix size: %d\nIterations: %d\nThreads: %d\nLines for each thread: %d\n", N, tsteps, num_threads, chunk_size);

    /* Variable declaration/allocation. */
    A = (DATA_TYPE(*)[N][N])polybench_alloc_data((N * N), sizeof(DATA_TYPE));
    B = (DATA_TYPE(*)[N][N])polybench_alloc_data((N * N), sizeof(DATA_TYPE));
    
    srand(seed);
    /* Initialize array(s). */
    init_array(N, POLYBENCH_ARRAY(A), POLYBENCH_ARRAY(B));
    if (dce) print_array(N, POLYBENCH_ARRAY(A));

    /* Start timer. */
    //   polybench_start_instruments;
    
    /* Initialize threads and initialize barrier. */
    pthread_t* threads = (pthread_t*)malloc(num_threads * sizeof(pthread_t));

    if (threads == NULL) {
        fprintf(stderr, "Error allocating memory for threads");
        exit(1);
    }

    pthread_barrier_init(&barrier, NULL, n_threads);

    for (i = 0; i < n_threads; i++) {
        int* thread_id = (int*)malloc(sizeof(int));if (thread_id == NULL) {
            fprintf(stderr, "Error allocating memory for thread ID\n");
            exit(1);
        }
        *thread_id = i;

        /* Run kernel. */
        if (pthread_create(&threads[i], NULL, kernel_jacobi_2d_parallel, (void*)thread_id) != 0) {
            fprintf(stderr, "Error creating thread %d", i);
            exit(1);
        }
    }

    if (DEBUG)
        fprintf(stdout, "All threads created\n");

    /* Wait for threads to finish and destroy barrier. */
    for (i = 0; i < n_threads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Error joining thread %d", i);
            exit(1);
        };
    }
    // free(threads);
    
    if (DEBUG)
        fprintf(stdout, "All threads destroyed\n");

    pthread_barrier_destroy(&barrier);

    /* Stop and print timer. */
    //   polybench_stop_instruments;
    //   polybench_print_instruments;

    /* Prevent dead-code elimination. All live-out data must be printed
        by the function call in argument. */
    //   polybench_prevent_dce(print_array(n, POLYBENCH_ARRAY(A)));
    //if (dce) print_array(N, POLYBENCH_ARRAY(A));

    /* Be clean. */
    POLYBENCH_FREE_ARRAY(A);
    POLYBENCH_FREE_ARRAY(B);
}
