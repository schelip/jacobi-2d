#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#include <polybench.h>

/* Main computational kernel. The whole function will be timed,
   including the call and return. */
static
void worker_mpi(int rank, int n_workers, int chunk_size, int tsteps) {
    // printf("[Worker] %d online\n", rank);
    int t, i, j;

    double A[N][N], B[N][N];

    int neigh_above = (rank == 1) ? 0 : rank - 1;
    int neigh_below = (rank == n_workers) ? 0 : rank + 1;

    int start_row = 1 + (rank - 1) * chunk_size;
    int end_row = (rank == n_workers) ? (N - 2) : (start_row + chunk_size) - 1;

    MPI_Status status;

    if (!neigh_above) {
        // printf("[Worker %d] receiving top border\n", rank);
        MPI_Recv(&A[0][1], N - 2, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, &status);
    }
    if (!neigh_below) {
        // printf("[Worker %d] receiving bottom border\n", rank);
        MPI_Recv(&A[N - 1][1], N - 2, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, &status);
    }

    // printf("[Worker %d] receiving [%d - %d] from coordinator\n", rank, start_row, end_row);
    for (int i = start_row; i <= end_row; i++)
        MPI_Recv(&A[i][0], N, MPI_DOUBLE, 0, TAG, MPI_COMM_WORLD, &status);

    // if (rank == 1) {
        // printf("[Worker %d] initial matrix\n", rank);
    //     print_array(N, A);
    // }

    for (t = 0; t < tsteps; t++) {
        if (neigh_above) {
            // printf("[Worker %d] sending [%d] to worker %d\n", rank, start_row, neigh_above);
            MPI_Send(&A[start_row][1], N - 2, MPI_DOUBLE, neigh_above, TAG, MPI_COMM_WORLD);
        }
        if (neigh_below) {
            // printf("[Worker %d] sending [%d] to worker %d\n", rank, end_row, neigh_below);
            MPI_Send(&A[end_row][1], N - 2, MPI_DOUBLE, neigh_below, TAG, MPI_COMM_WORLD);
        }
        if (neigh_below) {
            // printf("[Worker %d] receiving [%d] from worker %d\n", rank, end_row + 1, neigh_below);
            MPI_Recv(&A[end_row + 1][1], N - 2, MPI_DOUBLE, neigh_below, TAG, MPI_COMM_WORLD, &status);
        }
        if (neigh_above) {
            // printf("[Worker %d] receiving [%d] from worker %d\n", rank, start_row - 1, neigh_above);
            MPI_Recv(&A[start_row - 1][1], N - 2, MPI_DOUBLE, neigh_above, TAG, MPI_COMM_WORLD, &status);
        }
        // if (rank == 1) {
            // printf("[Worker %d] start of iteration %d\n", rank, t);
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

void jacobi_2d_mpi(int t, int dce, int seed, int argc, char *argv[]) {
    /* Start timer. */
    //   polybench_start_instruments;

    int rank, n_workers;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_workers);

    MPI_Comm_size(MPI_COMM_WORLD, &n_workers);
    n_workers--;

    int chunk_size = (N - 2) / n_workers;

    if (rank == 0) {
        // printf("[Coordinator] online\n");
        /* Variable declaration/allocation. */
        double A[N][N];

        /* Initialize array(s). */
        srand(seed);
        init_array(N, A);

        MPI_Status status;

        // printf("[Coordinator] sending top border\n");
        MPI_Send(&A[0][1], N - 2, MPI_DOUBLE, 1, TAG, MPI_COMM_WORLD);
        // printf("[Coordinator] sending bottom border\n");
        MPI_Send(&A[N - 1][1], N - 2, MPI_DOUBLE, n_workers, TAG, MPI_COMM_WORLD);

        for (int worker = 1; worker <= n_workers; worker++) {
            int start_row = 1 + (worker - 1) * chunk_size;
            int end_row = (worker == n_workers) ? (N - 2) : (start_row + chunk_size) - 1;
            // printf("[Coordinator] sending [%d - %d] to worker %d\n", start_row, end_row, worker);
            for (int i = start_row; i <= end_row; i++)
                MPI_Send(&A[i][0], N, MPI_DOUBLE, worker, TAG, MPI_COMM_WORLD);
        }

        for (int worker = 1; worker <= n_workers; worker++) {
            int start_row = 1 + (worker - 1) * chunk_size;
            int end_row = (worker == n_workers) ? (N - 2) : (start_row + chunk_size) - 1;
            // printf("[Coordinator] receiving [%d - %d] from worker %d\n", start_row, end_row, worker);
            for (int i = start_row; i <= end_row; i++)
                MPI_Recv(&A[i][1], N - 1, MPI_DOUBLE, worker, TAG, MPI_COMM_WORLD, &status);
        }

        /* Prevent dead-code elimination. All live-out data must be printed
            by the function call in argument. */
        //   polybench_prevent_dce(print_array(n, POLYBENCH_ARRAY(A)));
        if (dce) print_array(N, A);

        /* Stop and print timer. */
        //   polybench_stop_instruments;
        //   polybench_print_instruments;
    } else {
        /* Run worker. */
        worker_mpi(rank, n_workers, chunk_size, t);
    }

    MPI_Finalize();
}

int main(int argc, char *argv[]) {
    jacobi_2d_mpi(SMALL_DATASET, 1, 1, argc, argv);
}
