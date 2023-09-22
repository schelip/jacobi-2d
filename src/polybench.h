#ifndef POLYBENCH_H
#define POLYBENCH_H

#define N 8
#define SMALL_DATASET 5
#define MEDIUM_DATASET 400
#define LARGE_DATASET 600

#define MAX_VALUE 15

#define TAG 0

#define DEBUG 0

extern void *polybench_alloc_data(unsigned long long int n, int elt_size);
extern void init_array(int n, double A[N][N]);
extern void init_array_with_copy(int n, double A[N][N], double B[N][N]);
extern void print_array(int n, double A[N][N]);

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

/* The options we understand. */
extern struct argp_option options[];

extern void parse_args(int argc, char *argv[], struct arguments *arguments);

#endif // POLYBENCH_H
