#ifndef PRAGMAFORV_GENERATOR_H
#define PRAGMAFORV_GENERATOR_H

#include <math.h>
#include <time.h>

const long int N = 4000;

double randDouble(double min, double max) {
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

void GenerateMatrix(double* matrix) {
    for(int i = 0; i < N; i++){
        double string_el_sum = 0.0;
        for (int j = 0; j < i; j++) {
            matrix[i * N + j] = matrix[j * N + i];
        }
        for (int j = i; j < N; j++) {
            if (i == j) {
                matrix[i * N + j] = randDouble(-10, 10);
                string_el_sum += fabs(matrix[i * N + j]);
            }
        }
        matrix[i * N + i] = string_el_sum + randDouble(5, 10);
        // We want diagonal elements to weight more.
    }
}

void GenerateVector(double* vector) {
    for (long i = 0; i < N; i++) {
        vector[i] = randDouble(-20, 20);
    }
}

#endif //PRAGMAFORV_GENERATOR_H
