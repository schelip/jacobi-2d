#define N 5
#define SMALL_DATASET 200
#define MEDIUM_DATASET 400
#define LARGE_DATASET 600

#define DATA_TYPE double
#define DATA_PRINTF_MODIFIER "%0.2lf "

#define POLYBENCH_PADDING_FACTOR 0
#define POLYBENCH_C99_SELECT(x,y) x
#define POLYBENCH_2D(var, dim1, dim2, ddim1, ddim2) var[POLYBENCH_C99_SELECT(dim1,ddim1) + POLYBENCH_PADDING_FACTOR][POLYBENCH_C99_SELECT(dim2,ddim2) + POLYBENCH_PADDING_FACTOR]
#define POLYBENCH_DECL_VAR(x) (*x)
#define POLYBENCH_ALLOC_2D_ARRAY(n1, n2, type)		\
  (type(*)[n1 + POLYBENCH_PADDING_FACTOR][n2 + POLYBENCH_PADDING_FACTOR])polybench_alloc_data ((n1 + POLYBENCH_PADDING_FACTOR) * (n2 + POLYBENCH_PADDING_FACTOR), sizeof(type))
#define POLYBENCH_2D_ARRAY_DECL(var, type, dim1, dim2, ddim1, ddim2)	\
  type POLYBENCH_2D(POLYBENCH_DECL_VAR(var), dim1, dim2, ddim1, ddim2); \
  var = POLYBENCH_ALLOC_2D_ARRAY(POLYBENCH_C99_SELECT(dim1, ddim1), POLYBENCH_C99_SELECT(dim2, ddim2), type);
#define POLYBENCH_ARRAY(x) *x
#define POLYBENCH_FREE_ARRAY(x) free((void*)x);

void polybench(int tsteps, int dce);