#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

/*
 * Program demonstruje przetwarzanie potokowe z użyciem trzech procesów.
 * Proces 1: Wczytuje ciąg znaków z konsoli i wysyła go do procesu 2 przez potok.
 * Proces 2: Zamienia małe litery na duże i przekazuje ciąg do procesu 3 przez potok.
 * Proces 3: Odbiera ciąg i wyświetla go na ekranie.
 * Proces główny zarządza procesami i nie wykonuje operacji logicznych.
 */

#define MAX_STR 256

int main()
{
    int pipe1[2], pipe2[2];
    pid_t pid1, pid2, pid3;

    if (pipe(pipe1) == -1)
    {
        perror("Błąd tworzenia pierwszego potoku");
        exit(EXIT_FAILURE);
    }

    if (pipe(pipe2) == -1)
    {
        perror("Błąd tworzenia drugiego potoku");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();
    if (pid1 == 0)
    {
        char input[MAX_STR];
        close(pipe1[0]);
        close(pipe2[0]);
        close(pipe2[1]);

        printf("Wpisz ciąg znaków: ");
        fgets(input, MAX_STR, stdin);

        write(pipe1[1], input, strlen(input) + 1);
        close(pipe1[1]);
        exit(EXIT_SUCCESS);
    }

    pid2 = fork();
    if (pid2 == 0)
    {
        char buffer[MAX_STR];
        close(pipe1[1]);
        close(pipe2[0]);

        read(pipe1[0], buffer, MAX_STR);
        close(pipe1[0]);

        for (int i = 0; buffer[i]; i++)
        {
            buffer[i] = toupper(buffer[i]);
        }

        write(pipe2[1], buffer, strlen(buffer) + 1);
        close(pipe2[1]);
        exit(EXIT_SUCCESS);
    }

    pid3 = fork();
    if (pid3 == 0)
    {
        char buffer[MAX_STR];
        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[1]);

        read(pipe2[0], buffer, MAX_STR);
        printf("Odebrany ciąg: %s", buffer);
        close(pipe2[0]);
        exit(EXIT_SUCCESS);
    }

    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[0]);
    close(pipe2[1]);

    wait(NULL);
    wait(NULL);
    wait(NULL);

    return 0;
}