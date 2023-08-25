#ifndef POLYBENCH_H
#define POLYBENCH_H

#define N 19
#define SMALL_DATASET 5
#define MEDIUM_DATASET 400
#define LARGE_DATASET 600

#define DATA_TYPE double
#define DATA_PRINTF_MODIFIER "%0.2lf "

#define DEBUG 1

#define POLYBENCH_PADDING_FACTOR 0
#define POLYBENCH_2D(var, dim1, dim2) var[dim1 + POLYBENCH_PADDING_FACTOR][dim2 + POLYBENCH_PADDING_FACTOR]
#define POLYBENCH_DECL_VAR(x) (*x)
#define POLYBENCH_ALLOC_2D_ARRAY(n1, n2, type)		\
  (type(*)[n1 + POLYBENCH_PADDING_FACTOR][n2 + POLYBENCH_PADDING_FACTOR])polybench_alloc_data ((n1 + POLYBENCH_PADDING_FACTOR) * (n2 + POLYBENCH_PADDING_FACTOR), sizeof(type))
#define POLYBENCH_2D_ARRAY_DECL(var, type, dim1, dim2)	\
  type POLYBENCH_2D(POLYBENCH_DECL_VAR(var), dim1, dim2); \
  var = POLYBENCH_ALLOC_2D_ARRAY(dim1, dim2, type);
#define POLYBENCH_ARRAY(x) *x
#define POLYBENCH_FREE_ARRAY(x) free((void*)x);

extern void *polybench_alloc_data(unsigned long long int n, int elt_size);
extern void init_array(int n, DATA_TYPE POLYBENCH_2D(A, N, N), DATA_TYPE POLYBENCH_2D(B, N, N));
extern void print_array(int n, DATA_TYPE POLYBENCH_2D(A, N, N));

#endif // POLYBENCH_H
