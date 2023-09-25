#ifndef COMMON_H
#define COMMON_H

#define N 10
#define SMALL_DATASET 4
#define MEDIUM_DATASET 400
#define LARGE_DATASET 600

#define MAX_VALUE 15

#define TAG 0

#define DEBUG 1
#define DEBUG_THREAD 0
#define DEBUG_RANK 0

extern void init_array(double A[N][N]);
extern void init_array_with_copy(double A[N][N], double B[N][N]);
extern void print_array(double A[N][N]);

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

/* The options to be understood. Must be defined. */
extern struct argp_option options[];

extern int parse_args(int argc, char *argv[], struct arguments *arguments);

#endif // COMMON_H
