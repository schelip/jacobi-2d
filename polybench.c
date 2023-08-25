#include <stdio.h>
#include <stdlib.h>

#include <polybench.h>

static void* xmalloc (size_t num) {
    void* new = NULL;
    int ret = posix_memalign(&new, 32, num);
    if (!new || ret) {
        fprintf (stderr, "[PolyBench] posix_memalign: cannot allocate memory");
        exit (1);
    }
    return new;
}

void* polybench_alloc_data(unsigned long long int n, int elt_size) {
    /// FIXME: detect overflow!
    size_t val = n;
    val *= elt_size;
    void* ret = xmalloc(val);
    return ret;
}


/* Array initialization. */
void init_array(int n, DATA_TYPE POLYBENCH_2D(A, N, N), DATA_TYPE POLYBENCH_2D(B, N, N)) {
    int i, j;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            A[i][j] = ((DATA_TYPE) i*(j+2) + 2) / n;
            B[i][j] = ((DATA_TYPE) i*(j+3) + 3) / n;
        }
}

/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
void print_array(int n, DATA_TYPE POLYBENCH_2D(A, N, N)) {
    int i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
            fprintf(stdout, DATA_PRINTF_MODIFIER, A[i][j]);
        fprintf(stdout, "\n");  
    }
    fprintf(stdout, "\n");
}
