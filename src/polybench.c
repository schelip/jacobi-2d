#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <argp.h>
#include <string.h>

#include <polybench.h>

static void* xmalloc (size_t num) {
    void* new = NULL;
    int ret = posix_memalign(&new, 32, num);
    if (!new || ret) {
        fprintf(stderr, "[PolyBench] posix_memalign: cannot allocate memory");
        exit(1);
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
void init_array_with_copy(int n, double A[N][N], double B[N][N]) {
    int i, j;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            A[i][j] = roundf((MAX_VALUE * ((double)rand()) / RAND_MAX) * 100) / 100;
            B[i][j] = A[i][j];
        }
}

/* Array initialization. */
void init_array(int n, double A[N][N]) {
    int i, j;

    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            A[i][j] = roundf((MAX_VALUE * ((double)rand()) / RAND_MAX) * 100) / 100;
        }
}

/* DCE code. Must scan the entire live-out data.
   Can be used also to check the correctness of the output. */
void print_array(int n, double A[N][N]) {
    int i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
            fprintf(stdout, "%0.2lf\t", A[i][j]);
        fprintf(stdout, "\n");  
    }
    fprintf(stdout, "\n");
}

const char *argp_program_version = "jacobi 2d computation 1.0";

/* Program documentation. */
static char doc[] = "Application for analysis of Parallel Programming in the 2D Jacobi Computation Problem";

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


void parse_args(int argc, char *argv[], struct arguments* arguments) {
    /* Default values */
    arguments->size = SMALL;
    arguments->threads = 2;
    arguments->seed = 1;

    /* Parse our arguments; every option seen by parse_opt will
     be reflected in arguments. */
    argp_parse(&argp, argc, argv, 0, 0, arguments);
}

