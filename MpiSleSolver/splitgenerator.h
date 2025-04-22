#ifndef MPISLESOLVER_SPLITGENERATOR_H
#define MPISLESOLVER_SPLITGENERATOR_H

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <mpi.h>

const int N = 6400;

double randDouble(double min, double max) {
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

void GenerateLocalMatrix(double* local_A, int local_rows, int rank, int size) {
    for (int i = 0; i < local_rows; i++) {
        int global_i = rank * local_rows + i;
        double row_sum = 0.0;

        for (int j = 0; j < N; j++) {
            if (j == global_i) continue;
            double val = randDouble(-1, 1);
            local_A[i * N + j] = val;
            row_sum += fabs(val);
        }

        local_A[i * N + global_i] = row_sum + 1.0;
    }
}

void GenerateVector(double* vector) {
    for (long i = 0; i < N; i++) {
        vector[i] = randDouble(-20, 20);
    }
}

void printMatrix(double* A){
    printf("\n");

    for(unsigned int i = 0; i < N; i++) {
        for(unsigned int j = 0; j < N; j++) {
            printf("%lf ", A[i * N + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void printVector(const double* B, const char* name, int procRank, int procNum, int n){
    for (int numProc = 0; numProc < procNum; ++numProc) {
        if (procRank == numProc) {
            printf("%s in rank %d:\n", name, procRank);
            for (int i = 0; i < n; ++i) {
                printf("%lf\n", B[i]);
            }
            printf("\n");
        }
    }
}

#endif //MPISLESOLVER_SPLITGENERATOR_H
