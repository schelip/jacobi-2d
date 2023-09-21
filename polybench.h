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

#endif // POLYBENCH_H
