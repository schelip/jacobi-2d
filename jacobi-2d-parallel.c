#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#include <polybench.h>

#define NUM_THREADS 8
pthread_barrier_t barrier;

int tsteps;
DATA_TYPE POLYBENCH_2D(POLYBENCH_DECL_VAR(A), N, N);
DATA_TYPE POLYBENCH_2D(POLYBENCH_DECL_VAR(B), N, N);

void* kernel_jacobi_2d_parallel(void* arg) {
    int t, i, j;
    int thread_id = *((int *)arg);

    int chunk_size = (N - 2) / NUM_THREADS;
    int start_row = 1 + thread_id * chunk_size;
    int end_row = (thread_id == NUM_THREADS - 1) ? N - 2 : start_row + chunk_size;

    for (t = 0; t < tsteps; t++) {
        for (i = start_row; i < end_row; i++)
            for (j = 1; j < N - 1; j++)
                *B[i][j] = 0.2 * (*A[i][j] + *A[i][j - 1] + *A[i][1 + j] + *A[1 + i][j] + *A[i - 1][j]);
        pthread_barrier_wait(&barrier);

        for (i = start_row; i < end_row; i++)
            for (j = 1; j < N - 1; j++)
                *A[i][j] = *B[i][j];
        pthread_barrier_wait(&barrier);

        if (thread_id == 0)
            print_array(N, *A);
        pthread_barrier_wait(&barrier);
    }

    pthread_exit(NULL);
}

void jacobi_2d_parallel(int t, int dce) {
    tsteps = t;
    int i;

    /* Variable declaration/allocation. */
    A = (DATA_TYPE(*)[N][N])polybench_alloc_data((N * N), sizeof(DATA_TYPE));
    B = (DATA_TYPE(*)[N][N])polybench_alloc_data((N * N), sizeof(DATA_TYPE));

    /* Initialize array(s). */
    init_array(N, POLYBENCH_ARRAY(A), POLYBENCH_ARRAY(B));

    /* Start timer. */
    //   polybench_start_instruments;
    
    /* Initialize threads and initialize barrier. */
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    pthread_barrier_init(&barrier, NULL, NUM_THREADS);

    for (i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;

        /* Run kernel. */
        if (pthread_create(&threads[i], NULL, kernel_jacobi_2d_parallel, (void*)&thread_ids[i]) != 0) {
            fprintf(stderr, "Error creating thread %d", i);
        }
    }

    /* Wait for threads to finish and destroy barrier. */
    for (i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Error joining thread %d", i);
        };
    }

    pthread_barrier_destroy(&barrier);

    /* Stop and print timer. */
    //   polybench_stop_instruments;
    //   polybench_print_instruments;

    /* Prevent dead-code elimination. All live-out data must be printed
        by the function call in argument. */
    //   polybench_prevent_dce(print_array(n, POLYBENCH_ARRAY(A)));
    if (dce) print_array(N, POLYBENCH_ARRAY(A));

    /* Be clean. */
    POLYBENCH_FREE_ARRAY(A);
    POLYBENCH_FREE_ARRAY(B);
}
