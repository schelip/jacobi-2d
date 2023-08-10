#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <argp.h>

#include "polybench.h"

const char *argp_program_version = "program 1.0";

// Enum para as opções de tamanho da entrada
enum dataset_size { SMALL, MEDIUM, LARGE };

// Estrutura para armazenar os argumentos
struct arguments {
    enum dataset_size size;
    int threads;
};

// Opções de linha de comando
static struct argp_option options[] = {
    { "size", 'd', "SIZE", 0, "Dataset size option (SMALL, MEDIUM, LARGE)", 0 },
    { "threads", 't', "THREADS", 0, "Number of threads", 0 },
    { 0 }
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;

    switch (key) {
        case 'd':
            if (strcmp(arg, "SMALL") == 0)
                arguments->size = SMALL;
            else if (strcmp(arg, "MEDIUM") == 0)
                arguments->size = MEDIUM;
            else if (strcmp(arg, "LARGE") == 0)
                arguments->size = LARGE;
            else
                argp_error(state, "Tamanho inválido");
            break;
        case 't':
            arguments->threads = atoi(arg);
            break;
        case ARGP_KEY_ARG:
            argp_error(state, "Argumento posicional não é suportado");
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static char doc[] = "Application for analysis of Parallel Programming in the 2D Jacobi Computation Problem";

static struct argp argp = { options, parse_opt, 0, doc };

int main(int argc, char *argv[]) {
    struct arguments arguments;

    // Valores padrão para os argumentos
    arguments.size = SMALL;
    arguments.threads = 1;

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    // Exibe os valores dos argumentos
    polybench(arguments.size, 1);

    return 0;
}
