#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <omp.h>
#include <stdlib.h>

#include "generator.h"

const double epsilon = 0.00000001;
double parameter = 0.1;

void SimpleIterationsMethod() {
    double* prevX = (double*)malloc(N * sizeof(double));
    double* Ax = (double*)malloc(N * sizeof(double));
    double* b = (double*)malloc(N * sizeof(double));
    double* next_x = (double*)malloc(N * sizeof(double));
    double* matrix_A = (double*)malloc(N * N * sizeof(double));

    GenerateVector(b);
    GenerateVector(prevX);
    GenerateMatrix(matrix_A);

    double b_norm = 0;

    double normAxb = 0, res = 0, last_res = 0;
    int countIt = 1;
    short other_parameter_sign_tried = 0;
    double saveRes;
    int done = 0;

    double start = omp_get_wtime(); // system time counter

#pragma omp parallel shared(prevX, next_x, Ax, b, matrix_A, N, done)
    {
#pragma omp for reduction(+:b_norm)
        // treads create local versions of the b_norm vector, do
        // their part of job, and sum these parts in the end.
        for (int i = 0; i < N; i++) {
            b_norm += b[i] * b[i];
        }

        while (1) {
#pragma omp for
            for (int i = 0; i < N; i++) {
                Ax[i] = 0;
                for (int j = 0; j < N; j++) {
                    Ax[i] += matrix_A[i * N + j] * prevX[j];
                }
                Ax[i] -= b[i];
            }

            normAxb = 0;
#pragma omp for reduction(+:normAxb)
            for (int i = 0; i < N; i++) {
                normAxb += Ax[i] * Ax[i];
            }

#pragma omp for
            for (int i = 0; i < N; i++) {
                Ax[i] *= parameter;
                next_x[i] = prevX[i] - Ax[i];
            }

#pragma omp single nowait
            {
                b_norm = sqrt(b_norm);
                normAxb = sqrt(normAxb);
                res = normAxb / b_norm;

                printf("Iteration: %d, res = %lf\n", countIt, res);
                if (countIt == 1) saveRes = res;
                if (res <= epsilon) {
#pragma omp atomic write
                    done = 1;
                }
                if ((countIt > 100000 && last_res > res) || res == INFINITY) {
                    if (other_parameter_sign_tried == 1) {
                        printf("Does not converge. Chose other parameter \n");
                        done = 1;
                    } else {
                        parameter *= -1;
                        other_parameter_sign_tried = 1;
                        countIt = 0;
                        res = saveRes;
                    }
                }
                last_res = res;
                countIt++;
            }


#pragma omp for
            for (int i = 0; i < N; i++) {
                prevX[i] = next_x[i];
            }
#pragma omp barrier
#pragma omp flush(done)
            if (done) break;
        }
    }

    double end = omp_get_wtime();
    printf("time: %lf\n", end - start);
    printf("res = %lf, iterations: %d\n", res, countIt);

    free(Ax);
    free(next_x);
    free(prevX);
    free(b);
    free(matrix_A);
}

int main(int argc, char *argv[]) {
    srand(100);
    omp_set_num_threads(6);
    SimpleIterationsMethod();
    return 0;
}