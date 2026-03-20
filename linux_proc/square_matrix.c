#include "square_matrix.h"

square_matrix_t* create_square_matrix(const size_t size)
{
    square_matrix_t* square_matrix = malloc(sizeof(square_matrix_t));
    square_matrix->size = size;

    square_matrix->value = malloc(size * sizeof(value_t*));
    for(size_t i = 0; i < size; i++)
    {
        square_matrix->value[i] = calloc(size, sizeof(value_t));
    }

    return square_matrix;
}

void random_square_matrix(square_matrix_t* square_matrix)
{
    for(size_t i = 0; i < square_matrix->size; i++)
    {
        for(size_t j = 0; j < square_matrix->size; j++)
        {
            square_matrix->value[i][j] = ((RAND_MAX / 2) - rand()) % __INT8_MAX__;
        }
    }
}

void fprint_square_matrx(square_matrix_t* square_matrix, const char* file_name)
{
    FILE* file = fopen(file_name, "w");
    for(size_t i = 0; i < square_matrix->size; i++)
    {
        for(size_t j = 0; j < square_matrix->size; j++)
        {
            fprintf(file, "%lli ", square_matrix->value[i][j]);
        }
        fputc('\n', file);
    }
    fclose(file);
}

void free_square_matrix(square_matrix_t** square_matrix)
{
    for(size_t i = 0; i < (*square_matrix)->size; i++)
    {
        free((*square_matrix)->value[i]);
    }
    free((*square_matrix)->value);
    free(*square_matrix);

    *square_matrix = NULL;
}

square_matrix_t* power_square_matrix(square_matrix_t* square_matrix)
{
    square_matrix_t *result_matrix = create_square_matrix(square_matrix->size);
    for (size_t i = 0; i < square_matrix->size; i++)
    {
        for (size_t j = 0; j < square_matrix->size; j++)
        {
            for (size_t k = 0; k < square_matrix->size; k++)
            {
                result_matrix->value[i][j] += square_matrix->value[i][k] * square_matrix->value[k][j];
            }
        }
    }

    return result_matrix;
}