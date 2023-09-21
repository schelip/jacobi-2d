#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <polybench.h>

/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void kernel_jacobi_2d_serial(int tsteps, DATA_TYPE POLYBENCH_2D(A, N, N), DATA_TYPE POLYBENCH_2D(B, N, N)) {
    int t, i, j;

    for (t = 0; t < tsteps; t++) {
        for (i = 1; i < N - 1; i++)
	        for (j = 1; j < N - 1; j++)
	            B[i][j] = 0.2 * (A[i][j] + A[i][j-1] + A[i][1+j] + A[1+i][j] + A[i-1][j]);
        for (i = 1; i < N-1; i++)
	        for (j = 1; j < N - 1; j++)
	            A[i][j] = B[i][j];
    }
}

void jacobi_2d_serial(int t, int dce, int seed) {
  /* Variable declaration/allocation. */
  POLYBENCH_2D_ARRAY_DECL(A, DATA_TYPE, N, N);
  POLYBENCH_2D_ARRAY_DECL(B, DATA_TYPE, N, N);

  srand(seed);
  /* Initialize array(s). */
  init_array_with_copy(N, POLYBENCH_ARRAY(A), POLYBENCH_ARRAY(B));

  /* Start timer. */
//   polybench_start_instruments;

  /* Run kernel. */
  kernel_jacobi_2d_serial(t, POLYBENCH_ARRAY(A), POLYBENCH_ARRAY(B));

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