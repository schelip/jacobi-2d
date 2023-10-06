#ifndef JACOBI_2D_H
#define JACOBI_2D_H

/* Dataset sizes. */
#define N 20
#define SMALL_DATASET 120
#define MEDIUM_DATASET 240
#define LARGE_DATASET 480


/* Constants. */
#define MAX_VALUE 15
#define TAG 0


/* Debug config. */
#define DEBUG 0
#define DEBUG_THREAD 0
#define DEBUG_RANK 3


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

extern void init_grid(double a[N * N]);
extern void init_grid_with_copy(double a[N * N], double b[N * N]);
extern void print_grid(double a[N * N]);
extern void get_limits(int rank, int chunk_size, int n_workers, int *start_row, int *end_row);


/* Timing macros and functions. */
#define START_TIMER_MPI double start, end; start = MPI_Wtime();
#define STOP_TIMER_MPI end = MPI_Wtime(); float exec_time = end - start;

#define START_TIMER struct timeval start, end; gettimeofday(&start, NULL);
#define STOP_TIMER gettimeofday(&end, NULL); float exec_time = time_diff(&start, &end);
extern float time_diff(struct timeval *start, struct timeval *end);

#define PRINT_EXEC_TIME printf("%0.8f\n", exec_time);


/* Argument parsing */
#define THREAD_ARG_OPTIONS struct argp_option options[] = \
    { \
        {"size", 'd', "SIZE", 0, "Dataset size option (SMALL, MEDIUM, LARGE) - defines the number of iterations for the computation", 0}, \
        {"seed", 's', "SEED", 0, "Seed for the array initialization", 0}, \
        {"print-result", 'p', NULL, 0, "If present, will print the result of the computation; if not, will print execution time", 0}, \
        { "threads", 't', "THREADS", 0, "Number of threads for the parallel computation", 0 }, \
        {0} \
    };

#define ARG_OPTIONS struct argp_option options[] = \
    { \
        {"size", 'd', "SIZE", 0, "Dataset size option (SMALL, MEDIUM, LARGE) - defines the number of iterations for the computation", 0}, \
        {"seed", 's', "SEED", 0, "Seed for the array initialization", 0}, \
        {"print-result", 'p', NULL, 0, "If present, will print the result of the computation; if not, will print execution time", 0}, \
        {0} \
    };

#define VERIFY_NUM_PROCESSES if (num_workers > N - 2) \
    { \
        if (rank == 0) \
            printf("Error: number of processes cannot be greater than the dimension of the matrix (not counting borders)\n"); \
        MPI_Finalize(); \
        exit(EXIT_FAILURE); \
    }

#define VERIFY_NUM_THREADS if (num_threads > N - 2) \
    { \
        printf("Error: number of threads cannot be greater than the dimension of the matrix (not counting borders)\n"); \
        exit(EXIT_FAILURE); \
    }

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
    int print_result;
};

/* The options to be understood. Must be defined on each program. */
extern struct argp_option options[];

extern int parse_args(int argc, char *argv[], struct arguments *arguments);

#endif // COMMON_H
