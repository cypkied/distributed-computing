#ifndef SQUARE_MATRIX_H
#define SQUARE_MATRIX_H

#include <stdio.h>
#include <stdlib.h>

typedef long long value_t;

typedef struct square_matrix_t
{
    size_t size;
    value_t** value;
} square_matrix_t;

square_matrix_t* create_square_matrix(const size_t size);

void random_square_matrix(square_matrix_t* square_matrix);

void fprint_square_matrx(square_matrix_t* square_matrix, const char* file_name);

void free_square_matrix(square_matrix_t** square_matrix);

square_matrix_t* power_square_matrix(square_matrix_t* square_matrix);

#endif