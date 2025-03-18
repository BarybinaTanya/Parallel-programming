#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <omp.h>
#include <stdlib.h>

#include "generator.h"

typedef short flag;

const double epsilon = 0.00000001;
double parameter = 0.1;

double EuclideanNorm(const double* u){
    double norm = 0;
#pragma omp parallel for reduction (+:norm)
    for (int i = 0; i < N; i++){
        norm += u[i] * u[i];
    }
    return sqrt(norm);
}

void SubVectors(const double* vector_a, const double* vector_b, double* vector_c){
#pragma omp parallel for
    for (int i = 0; i < N; i++) {
        vector_c[i] = vector_a[i] - vector_b[i];
    }
}

void MultiplyMatrixToVector(const double* matrix_A, const double* vector_b, double* vector_result) {
    unsigned int i;
#pragma omp parallel for
    for (i = 0; i < N; i++) {
        vector_result[i] = 0;
        for (int j = 0; j < N; j++) {
            vector_result[i] += matrix_A[i * N + j] * vector_b[j];
        }
    }
}

void MultiplyMatrixToScalar(double* matrix_A, double scalar){
    int i;
#pragma omp parallel for
    for (i = 0; i < N; ++i) {
        matrix_A[i] = matrix_A[i] * scalar;
    }
}


void SimpleIterationsMethodCore(double* matrix_A, double* Ax, double* prevX, double* b, double* normAxb,
                                double* next_x, double* res, const double* b_norm) {
    MultiplyMatrixToVector(matrix_A, prevX, Ax); // matrix_A*xn
    SubVectors(Ax, b, Ax); // matrix_A*xn - b
    *normAxb = EuclideanNorm(Ax); // matrix_A*xn - b
    MultiplyMatrixToScalar(Ax, parameter); // TAU*(matrix_A*xn - b)
    SubVectors(prevX, Ax, next_x); // xn - TAU * (matrix_A*xn - b)
    *res = *normAxb / *b_norm;

}

void SimpleIterationsMethod() {
    double* prevX = (double*)malloc(N * sizeof(double));
    double* Ax = (double*)malloc(N * sizeof(double));

    double normAxb = 0; // matrix_A*xn - b
    double b_norm = 0;
    double saveRes;
    double res = 0;
    double last_res;
    double start, end;

    double* b = (double*)malloc(N * sizeof(double));
    double* next_x = (double*)malloc(N * sizeof(double));
    double* matrix_A = (double*)malloc(N * N * sizeof(double));

    GenerateVector(b);
    GenerateVector(prevX);
    GenerateMatrix(matrix_A);

    b_norm = EuclideanNorm(b);

    SimpleIterationsMethodCore(matrix_A, Ax, prevX, b, &normAxb, next_x, &res, &b_norm);

    saveRes = normAxb / b_norm;
    last_res = res;

    int countIt = 1;

    start = omp_get_wtime();

    flag other_parameter_sign_tried = 0;

    while (res > epsilon) {
#pragma omp parallel for
        for (long i = 0; i < N; i++) {
            prevX[i] = next_x[i];
        }
        SimpleIterationsMethodCore(matrix_A, Ax, prevX, b, &normAxb, next_x, &res, &b_norm);
        countIt++;

        if ((countIt > 100000 && last_res > res) || res == INFINITY) {
            if (other_parameter_sign_tried == 1) {
                printf("Does not converge. Chose other parameter \n");
                res = 0;
            } else {
                parameter = parameter * -1;
                other_parameter_sign_tried = 1;
                countIt = 0;
                res = saveRes;
            }
        }
        last_res = res;
        printf("init res = %lf, epsil = %lf\n", res, epsilon);
    }

    end = omp_get_wtime();
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
    omp_set_num_threads(8);
    SimpleIterationsMethod();
    return 0;
}

