#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

double* read_points(FILE *f, int* n_out, int *d_out) {
    int32_t n, d;

    if (fread(&n, sizeof(int32_t), 1, f) != 1) {
        return NULL;
    }
    if (fread(&d, sizeof(int32_t), 1, f) != 1) {
        return NULL;
    }

    if (n <= 0 || d <= 0) {
        return NULL;
    }

    double *data = malloc((size_t)n * (size_t)d * sizeof(double));
    if (!data) {
        return NULL;
    }

    size_t total = (size_t)n * (size_t)d;
    if (fread(data, sizeof(double), total, f) != total) {
        free(data);
        return NULL;
    }

    *n_out = n;
    *d_out = d;
    return data;
}

int* read_indexes(FILE *f, int *n_out, int *k_out) {
    int32_t n, k;

    if (fread(&n, sizeof(int32_t), 1, f) != 1) {
        return NULL;
    }
    if (fread(&k, sizeof(int32_t), 1, f) != 1) {
        return NULL;
    }

    if (n <= 0 || k <= 0) {
        return NULL;
    }

    size_t total = (size_t)n * (size_t)k;
    int *data = malloc(total * sizeof(int));
    if (!data) {
        return NULL;
    }

    // Read as int32_t and cast to int.
    for (size_t i = 0; i < total; i++) {
        int32_t tmp;
        if (fread(&tmp, sizeof(int32_t), 1, f) != 1) {
            free(data);
            return NULL;
        }
        data[i] = (int)tmp;
    }

    *n_out = n;
    *k_out = k;
    return data;
}

int write_points(FILE *f, int32_t n, int32_t d, double *data) {
    if (fwrite(&n, sizeof(int32_t), 1, f) != 1) {
        return 1;
    }
    if (fwrite(&d, sizeof(int32_t), 1, f) != 1) {
        return 1;
    }

    size_t total = (size_t)n * (size_t)d;
    if (fwrite(data, sizeof(double), total, f) != total) {
        return 1;
    }

    return 0;
}

int write_indexes(FILE *f, int32_t n, int32_t k, int *data) {
    if (fwrite(&n, sizeof(int32_t), 1, f) != 1) {
        return 1;
    }
    if (fwrite(&k, sizeof(int32_t), 1, f) != 1) {
        return 1;
    }

    size_t total = (size_t)n * (size_t)k;
    for (size_t i = 0; i < total; i++) {
        int32_t tmp = (int32_t)data[i];
        if (fwrite(&tmp, sizeof(int32_t), 1, f) != 1) {
            return 1;
        }
    }

    return 0;
}
