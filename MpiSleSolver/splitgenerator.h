#ifndef MPISLESOLVER_SPLITGENERATOR_H
#define MPISLESOLVER_SPLITGENERATOR_H

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <mpi.h>

const int N = 40;

double randDouble(double min, double max) {
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

void GenerateLocalMatrix(double* local_A, int local_rows, int rank, int size) {
    for (int i = 0; i < local_rows; i++) {
        int global_i = rank * local_rows + i;
        double string_el_sum = 0.0;

        for (int j = 0; j < N; j++) {
            if (global_i >= j) { // Fill elements under diagonal of our square matrix.
                local_A[i * N + j] = randDouble(-10, 10);
                int receive_process = j / local_rows;
                if (receive_process != rank) {
                    MPI_Send(&local_A[i * N + j], 1, MPI_DOUBLE,
                    receive_process, global_i * N + j, MPI_COMM_WORLD);
                } else {
                    local_A[i * N + j] = 0.0;
                }
                if (global_i == j) {
                    local_A[i * N + j] += 100;
                }
            }
        }
    }

    for (int i = 0; i < local_rows; ++i) {
        int global_i = rank * local_rows + i;
        for (int j = global_i + 1; j < N; ++j) {
            // we need matrix_A[j * N + i].
            int owner_process = j / local_rows;
            // in local matrix of the process it will be j % local_rows row.
            // so we are searching for local_A[j % local_rows][global_i] (or local_A[(j % local_rows) * N + global_i])
            int owner_local_index = j % local_rows;
            if (owner_process != rank) {
                MPI_Recv(&local_A[i * N + j], 1, MPI_DOUBLE, owner_process,
                         j * N + global_i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            } else {
                local_A[i * N + j] = local_A[owner_local_index * N + global_i];
            }
        }
    }
}

void GenerateVector(double* vector, int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = randDouble(-20, 20);
    }
}


#endif //MPISLESOLVER_SPLITGENERATOR_H
