#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
#include <time.h>
#include <sys/stat.h>
#include "square_matrix.h"

void ensure_directory_exists(const char *dir)
{
    struct stat st = {0};
    if (stat(dir, &st) == -1)
    {
        mkdir(dir, 0700);
    }
}

void task(const size_t SIZE, const size_t i)
{
    square_matrix_t *input_matrix = create_square_matrix(SIZE);
    random_square_matrix(input_matrix);

    char buff[255] = "";

    sprintf(buff, "./input/matrix%li.txt", i);
    fprint_square_matrx(input_matrix, buff);
    square_matrix_t *result_matrix = power_square_matrix(input_matrix);
    sprintf(buff, "./result/matrix%li.txt", i);
    fprint_square_matrx(result_matrix, buff);

    free_square_matrix(&input_matrix);
    free_square_matrix(&result_matrix);
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Użycie: %s <SIZE>\n", argv[0]);
        return EXIT_FAILURE;
    }

    size_t number = (size_t)atoi(argv[1]); // Konwersja argumentu na liczbę
    if (number <= 0)
    {
        fprintf(stderr, "Błąd: SIZE musi być liczbą większą od 0\n");
        return EXIT_FAILURE;
    }

    ensure_directory_exists("./input");
    ensure_directory_exists("./result");

    const size_t SIZE = 100;

    pid_t pid1 = fork();
    if (0 == pid1)
    {
        srand(time(NULL));
        for (size_t i = 0; i < number; i += 2)
        {
            task(SIZE, i);
        }
        exit(0);
    }

    pid_t pid2 = fork();
    if (0 == pid2)
    {
        srand(time(NULL) + 1);
        for (size_t i = 1; i < number; i += 2)
        {
            task(SIZE, i);
        }
        exit(0);
    }

    wait(NULL);
    wait(NULL);

    return 0;
}