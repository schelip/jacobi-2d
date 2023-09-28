#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <argp.h>

#include <common.h>

/* Variable declaration. */
DECLARE_GRIDS()

/* Main computational kernel. */
static void
kernel_jacobi_2d_serial(int tsteps) {
    int t, i, j;

    PREPARE_GRIDS()

    for (t = 0; t < tsteps; t++)
    {
        for (i = 1; i < N - 1; i++)
	        for (j = 1; j < N - 1; j++)
	            B EL(i, j) = 0.2 * (A EL(i, j) + A EL(i, j - 1) + A EL(i, 1 + j) + A EL(1 + i, j) + A EL(i - 1, j));

        if (DEBUG)
        {
            printf("Iter %d\n", t);
            print_grid(B);
        }

        SWAP_GRIDS()
    }
}

static void
jacobi_2d_serial(int tsteps, int seed) {
    /* Initialize array(s). */
    srand(seed);

    init_grid_with_copy(INITIAL_GRID, AUX_GRID);

    if (DEBUG)
        print_grid(INITIAL_GRID);

    /* Run kernel. */
    kernel_jacobi_2d_serial(tsteps);

    if (DEBUG)
        print_grid(RESULT_GRID);
}

/* The options we understand. */
struct argp_option options[] =
{
    {"size", 'd', "SIZE", 0, "Dataset size option (SMALL, MEDIUM, LARGE) - defines the number of iterations for the computation", 0},
    {"seed", 's', "SEED", 0, "Seed for the array initialization", 0},
    {0}
};

int
main(int argc, char *argv[]) {
    struct arguments arguments;
    parse_args(argc, argv, &arguments);
    jacobi_2d_serial(arguments.size, arguments.seed);
    exit(0);
}
