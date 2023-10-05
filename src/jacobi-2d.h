#ifndef JACOBI_2D_H
#define JACOBI_2D_H

/* Dataset sizes. */
#define N 10
#define SMALL_DATASET 4
#define MEDIUM_DATASET 240
#define LARGE_DATASET 480

/* Constants. */
#define MAX_VALUE 15
#define TAG 0

/* Debug config. */
#define DEBUG 1
#define DEBUG_THREAD -1
#define DEBUG_RANK 1

/* Grid macros and functions. */
#define EL(i, j) [((i) * N) + (j)]

#define A grid[current]
#define B grid[next]
#define INITIAL_GRID grid[0]
#define AUX_GRID grid[1]

#define RESULT_GRID grid[tsteps % 2]
#define DECLARE_GRIDS static double grid[2][N * N];
#define SWAP_GRIDS current = next; next = 1 - next;
#define PREPARE_GRIDS int current = 0, next = 1;

extern void init_array(double a[N][N]);
extern void init_array_with_copy(double a[N][N], double b[N][N]);
extern void print_array(double a[N][N]);
extern void init_grid(double a[N * N]);
extern void init_grid_with_copy(double a[N * N], double b[N * N]);
extern void print_grid(double a[N * N]);
extern void get_limits(int rank, int chunk_size, int n_workers, int *start_row, int *end_row);

/* Timing macros and functions. */
#define START_TIMER_MPI double start, end; start = MPI_Wtime();
#define STOP_TIMER_MPI end = MPI_Wtime(); if (rank == 0) printf("%0.8f\n", end - start);

#define START_TIMER struct timeval start, end; gettimeofday(&start, NULL);
#define STOP_TIMER gettimeofday(&end, NULL); printf("%0.8f\n", time_diff(&start, &end));
extern float time_diff(struct timeval *start, struct timeval *end);

/* Enumeration for the dataset size available options */
enum dataset_size {
    SMALL = SMALL_DATASET,
    MEDIUM = MEDIUM_DATASET,
    LARGE = LARGE_DATASET
};

/* Used by main to communicate with parse_opt. */
struct arguments {
    enum dataset_size size;
    int threads;
    int seed;
};

/* The options to be understood. Must be defined on each program. */
extern struct argp_option options[];

extern int parse_args(int argc, char *argv[], struct arguments *arguments);

#endif // COMMON_H
