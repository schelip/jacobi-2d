#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <argp.h>
#include <sys/time.h>

#include <jacobi-2d.h>

#define UTIL

/* Grid initialization. */
void
init_grid_with_copy(double a[N * N], double b[N * N])
{
    int i, j;

    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
        {
            a EL(i, j) = roundf((MAX_VALUE * ((double)rand()) / RAND_MAX) * 100) / 100;
            b EL(i, j) = a EL(i, j);
        }
}

/* Grid initialization. */
void
init_grid(double a[N * N])
{
    int i, j;

    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            a EL(i, j) = roundf((MAX_VALUE * ((double)rand()) / RAND_MAX) * 100) / 100;
}

/* Prints grid values. */
void
print_grid(double a[N * N])
{
    int i, j;

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
            fprintf(stdout, "%0.2lf\t", a EL(i, j));
        fprintf(stdout, "\n");  
    }
    fprintf(stdout, "\n");
}

/* Calculates the first and last row indexes for the chunk of a given rank. */
void
get_limits(int rank, int chunk_size, int n_workers, int *start_row, int *end_row)
{
    *start_row = 1 + rank * chunk_size;
    *end_row = (rank == n_workers - 1) ? (N - 2) : (*start_row + chunk_size) - 1;
}

/* Timing function. */
float
time_diff(struct timeval *start, struct timeval *end)
{
    return (end->tv_sec - start->tv_sec) + 1e-6 * (end->tv_usec - start->tv_usec);
}

/* Program version. */
const char *argp_program_version = "jacobi 2d computation 1.0";

/* Program documentation. */
static char doc[] = "Application for analysis of Parallel Programming in the 2D Jacobi Computation Problem";

/* Parse a single option. */
static error_t
parse_opt(int key, char *arg, struct argp_state *state)
{
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
        case 'p':
            arguments->print_result = 1;
            break;
        case ARGP_KEY_ARG:
            argp_error(state, "Positional argument is not supported");
            break;
        case ARGP_KEY_END:
            // Se a opção "print-result" não foi definida, defina-a como 0
            if (arguments->print_result != 1) {
                arguments->print_result = 0;
            }
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, 0, doc, NULL, NULL, NULL };

/* Parses arguments passed in command line, or uses default values. */
int
parse_args(int argc, char *argv[], struct arguments* arguments)
{
    /* Default values */
    arguments->size = SMALL;
    arguments->threads = 2;
    arguments->seed = 1;

    /* Parse our arguments; every option seen by parse_opt will
     be reflected in arguments. */
    return argp_parse(&argp, argc, argv, 0, 0, arguments);
}

