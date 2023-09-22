#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <argp.h>

#include <polybench.h>

static
void get_limits(int rank, int chunk_size, int n_workers, int *start_row, int *end_row) {
    *start_row = 1 + rank * chunk_size;
    *end_row = (rank == n_workers) ? (N - 2) : (*start_row + chunk_size) - 1;
}

/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void worker_mpi(int rank, int n_workers, int chunk_size) {
    // printf("[Worker] %d online\n", rank);
    int t, tsteps, i, j, neigh_above, neigh_below, start_row, end_row;
    double A[N][N], B[N][N];

    get_limits(rank, chunk_size, n_workers, &start_row, &end_row);

    neigh_above = rank - 1;
    neigh_below = (rank == n_workers) ? 0 : rank + 1;

    MPI_Status status;

    MPI_Bcast(&tsteps, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // if (!neigh_above) {
    //     printf("[Worker %d] receiving top border\n", rank);
    //     MPI_Recv(&A[0][1], N - 2, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, &status);
    // }
    if (!neigh_below) {
        // printf("[Worker %d] receiving bottom border\n", rank);
        MPI_Recv(&A[N - 1][1], N - 2, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, &status);
    }

    // printf("[Worker %d] receiving [%d - %d] from coordinator\n", rank, start_row, end_row);
    for (int i = start_row; i <= end_row; i++)
        MPI_Recv(&A[i][0], N, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, &status);

    // if (rank == 1) {
        // printf("[Worker %d] initial matrix\n", rank);
        // print_array(N, A);
    // }

    for (t = 0; t < tsteps; t++) {
        // if (neigh_above) {
            // printf("[Worker %d] sending [%d] to worker %d\n", rank, start_row, neigh_above);
            MPI_Send(&A[start_row][1], N - 2, MPI_DOUBLE, neigh_above, TAG, MPI_COMM_WORLD);
        // }
        if (neigh_below) {
            // printf("[Worker %d] sending [%d] to worker %d\n", rank, end_row, neigh_below);
            MPI_Send(&A[end_row][1], N - 2, MPI_DOUBLE, neigh_below, TAG, MPI_COMM_WORLD);
            // printf("[Worker %d] receiving [%d] from worker %d\n", rank, end_row + 1, neigh_below);
            MPI_Recv(&A[end_row + 1][1], N - 2, MPI_DOUBLE, neigh_below, TAG, MPI_COMM_WORLD, &status);
        }
        // if (neigh_above) {
            // printf("[Worker %d] receiving [%d] from worker %d\n", rank, start_row - 1, neigh_above);
            MPI_Recv(&A[start_row - 1][1], N - 2, MPI_DOUBLE, neigh_above, TAG, MPI_COMM_WORLD, &status);
        // }
        // if (rank == 1) {
        //     printf("[Worker %d] start of iteration %d\n", rank, t);
        //     print_array(N, A);
        // }

        // printf("[Worker %d] calculating [%d - %d] (iter %d)\n", rank, start_row, end_row, t);
        for (i = start_row; i <= end_row; i++)
	        for (j = 1; j < N - 1; j++)
	            B[i][j] = 0.2 * (A[i][j] + A[i][j - 1] + A[i][1 + j] + A[1 + i][j] + A[i - 1][j]);
        for (i = 1; i <= end_row; i++)
	        for (j = 1; j < N - 1; j++)
	            A[i][j] = B[i][j];
        // if (rank == 1) {
            // printf("[Worker %d] end of iteration %d\n", rank, t);
        //     print_array(N, A);
        // }
    }

    // printf("[Worker %d] sending final [%d - %d] to coordinator\n", rank, start_row, end_row);
    for (int i = start_row; i <= end_row; i++)
        MPI_Send(&A[i][1], N - 1, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD);
}

void coordinator_mpi(int tsteps, int dce, int seed, int n_workers, int chunk_size) {
    // printf("[Coordinator] online\n");
    /* Variable declaration/allocation. */
    int t, i, j, start_row, end_row, worker;
    double A[N][N], B[N][N];

    /* Initialize array(s). */
    srand(seed);
    init_array(N, A);

    MPI_Status status;

    MPI_Bcast(&tsteps, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // printf("[Coordinator] sending top border\n");
    // MPI_Send(&A[0][1], N - 2, MPI_DOUBLE, 1, TAG, MPI_COMM_WORLD);
    // printf("[Coordinator] sending bottom border to worker %d\n", n_workers);
    MPI_Send(&A[N - 1][1], N - 2, MPI_DOUBLE, n_workers, TAG, MPI_COMM_WORLD);

    for (worker = 1; worker <= n_workers; worker++) {
        get_limits(worker, chunk_size, n_workers, &start_row, &end_row);
        // printf("[Coordinator] sending [%d - %d] to worker %d\n", start_row, end_row, worker);
        for (i = start_row; i <= end_row; i++)
            MPI_Send(&A[i][0], N, MPI_DOUBLE, worker, TAG, MPI_COMM_WORLD);
    }

    start_row = 1;
    end_row = chunk_size;
    for (t = 0; t < tsteps; t++) {
        // printf("[Worker %d] sending [%d] to worker %d\n", 0, end_row, 1);
        MPI_Send(&A[end_row][1], N - 2, MPI_DOUBLE, 1, TAG, MPI_COMM_WORLD);
        // printf("[Worker %d] receiving [%d] from worker %d\n", 0, end_row + 1, 1);
        MPI_Recv(&A[end_row + 1][1], N - 2, MPI_DOUBLE, 1, TAG, MPI_COMM_WORLD, &status);
        // if (rank == 1) {
            // printf("[Worker %d] start of iteration %d\n", rank, t);
        //     print_array(N, A);
        // }

        // printf("[Worker %d] calculating [%d - %d] (iter %d)\n", 0, start_row, end_row, t);
        for (i = 1; i <= end_row; i++)
	        for (j = 1; j < N - 1; j++)
	            B[i][j] = 0.2 * (A[i][j] + A[i][j - 1] + A[i][1 + j] + A[1 + i][j] + A[i - 1][j]);
        for (i = 1; i <= end_row; i++)
	        for (j = 1; j < N - 1; j++)
	            A[i][j] = B[i][j];
        // if (rank == 1) {
            // printf("[Worker %d] end of iteration %d\n", rank, t);
        //     print_array(N, A);
        // }
    }

    for (worker = 1; worker <= n_workers; worker++) {
        get_limits(n_workers, chunk_size, n_workers, &start_row, &end_row);
        // printf("[Coordinator] receiving [%d - %d] from worker %d\n", start_row, end_row, worker);
        for (i = start_row; i <= end_row; i++)
            MPI_Recv(&A[i][1], N - 1, MPI_DOUBLE, worker, TAG, MPI_COMM_WORLD, &status);
    }
    
    if (dce) print_array(N, A);
}

/* The options we understand. */
struct argp_option options[] = {
    { "size", 'd', "SIZE", 0, "Dataset size option (SMALL, MEDIUM, LARGE) - defines the number of iterations for the computation", 0 },
    { "processes", 'np', "PROCESSES", 0, "Number of processes for MPI computation", 0 },
    { "seed", 's', "SEED", 0, "Seed for the array initialization", 0 },
    { 0 }
};

int main(int argc, char *argv[]) {
    int rank, n_workers, chunk_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_workers);
    n_workers--;

    chunk_size = (N - 2) / n_workers;

    if (rank == 0) {
        struct arguments arguments;
        parse_args(argc, argv, &arguments);
        coordinator_mpi(arguments.size, 1, arguments.seed, n_workers, chunk_size);
    } else {
        worker_mpi(rank, n_workers, chunk_size);
    }

    MPI_Finalize();
    exit(0);
}
