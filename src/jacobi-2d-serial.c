#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <argp.h>
#include <sys/time.h>

#include <jacobi-2d.h>

ARG_OPTIONS

/* Variable declaration. */
static int tsteps;

DECLARE_GRIDS

/* Main computational kernel. */
static void
kernel_jacobi_2d_serial(int tsteps) {
    int t, i, j;

    PREPARE_GRIDS

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

        SWAP_GRIDS
    }
}

static void
jacobi_2d_serial() {
    /* Initialize array(s). */

    init_grid_with_copy(INITIAL_GRID, AUX_GRID);

    if (DEBUG)
        print_grid(INITIAL_GRID);

    /* Run kernel. */
    kernel_jacobi_2d_serial(tsteps);

    if (DEBUG)
        print_grid(RESULT_GRID);
}

int
main(int argc, char *argv[])
{
    START_TIMER

    struct arguments arguments;
    parse_args(argc, argv, &arguments);

    tsteps = arguments.size;
    srand(arguments.seed);

    jacobi_2d_serial();

    STOP_TIMER
    
    if (arguments.print_result)
        print_grid(RESULT_GRID);
    else
        PRINT_EXEC_TIME

    exit(EXIT_SUCCESS);
}
