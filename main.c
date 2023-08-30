#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <argp.h>

#include <polybench.h>

void jacobi_2d_serial(int t, int dce);
void jacobi_2d_parallel(int t, int dce, int n_threads);

const char *argp_program_version = "jacobi 2d computation 1.0";

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

/* Program documentation. */
static char doc[] = "Application for analysis of Parallel Programming in the 2D Jacobi Computation Problem";

/* The options we understand. */
static struct argp_option options[] = {
    { "size", 'd', "SIZE", 0, "Dataset size option (SMALL, MEDIUM, LARGE) - defines the number of iterations for the computation", 0 },
    { "threads", 't', "THREADS", 0, "Number of threads for the parallel computation", 0 },
    { "seed", 's', "SEED", 0, "Seed for the array initialization", 0 },
    { 0 }
};

/* Parse a single option. */
static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;

    switch (key) {
        case 'd':
            if (strcmp(arg, "SMALL") == 0)
                arguments->size = SMALL_DATASET;
            else if (strcmp(arg, "MEDIUM") == 0)
                arguments->size = MEDIUM_DATASET;
            else if (strcmp(arg, "LARGE") == 0)
                arguments->size = LARGE_DATASET;
            else
                argp_error(state, "Invalid size option");
            break;
        case 't':
            arguments->threads = atoi(arg);
            break;
        case 's':
            arguments->seed = atoi(arg);
            break;
        case ARGP_KEY_ARG:
            argp_error(state, "Positional argument is not supported");
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, 0, doc, NULL, NULL, NULL };

int main(int argc, char *argv[]) {
    struct arguments arguments;

    /* Default values */
    arguments.size = SMALL;
    arguments.threads = 2;
    arguments.seed = 1;

    /* Parse our arguments; every option seen by parse_opt will
     be reflected in arguments. */
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    srand(arguments.seed);

    // jacobi_2d_parallel(arguments.size, 1, arguments.threads);
    jacobi_2d_serial(arguments.size, 1);

    exit(0);
}
