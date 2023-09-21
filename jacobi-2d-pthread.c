#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#include <polybench.h>

pthread_barrier_t barrier;

static int tsteps, num_threads, chunk_size;
static double A[N][N], B[N][N];

    
void* worker_pthread(void* arg) {
    int t, i, j;
    int thread_id = *((int*)arg);

    int start_row = 1 + thread_id * chunk_size;
    int end_row = (thread_id == num_threads - 1) ? (N - 1) : (start_row + chunk_size);

    for (t = 0; t < tsteps; t++) {
        for (i = start_row; i < end_row; i++)
            for (j = 1; j < N - 1; j++) {
                B[i][j] = 0.2 * (A[i][j] + A[i][j - 1] + A[i][1 + j] + A[1 + i][j] + A[i - 1][j]);
            }
        pthread_barrier_wait(&barrier);

        for (i = start_row; i < end_row; i++)
            for (j = 1; j < N - 1; j++)
                A[i][j] = B[i][j];
        pthread_barrier_wait(&barrier);
    }

    return (void*)NULL;
}

void jacobi_2d_pthread(int t, int dce, int n_threads, int seed) {
    int i;

    tsteps = t;
    num_threads = n_threads;
    chunk_size = (N - 2) / num_threads;

    srand(seed);
    /* Initialize array(s). */
    init_array_with_copy(N, A, B);

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
        if (pthread_create(&threads[i], NULL, worker_pthread, (void*)thread_id) != 0) {
            fprintf(stderr, "Error creating thread %d", i);
            exit(1);
        }
    }

    /* Wait for threads to finish and destroy barrier. */
    for (i = 0; i < n_threads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            fprintf(stderr, "Error joining thread %d", i);
            exit(1);
        };
    }
    
    pthread_barrier_destroy(&barrier);

    /* Stop and print timer. */
    //   polybench_stop_instruments;
    //   polybench_print_instruments;

    /* Prevent dead-code elimination. All live-out data must be printed
        by the function call in argument. */
    //   polybench_prevent_dce(print_array(n, POLYBENCH_ARRAY(A)));
    if (dce) print_array(N, A);
}
