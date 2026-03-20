/*
 * Program wyszukujący liczby pierwsze przy użyciu wątków POSIX.
 * Wątek główny tworzy N wątków potomnych, przekazując każdej kolejnej liczbie
 * całkowitej z zadanego przedziału do sprawdzenia. Wątki potomne używają
 * algorytmu sprawdzania, czy liczba jest pierwsza, i wyświetlają liczby pierwsze.
 * Program kończy działanie po sprawdzeniu wszystkich liczb w przedziale.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct
{
    int number;
} ThreadData;

int is_prime(int n)
{
    if (n <= 1)
        return 0;
    if (n == 2)
        return 1;
    if (n % 2 == 0)
        return 0;

    for (int i = 3; i * i <= n; i += 2)
    {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

void *thread_func(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    if (is_prime(data->number))
    {
        printf("Liczba pierwsza: %d (wątek %lu)\n", data->number, pthread_self());
    }
    free(data);
    return NULL;
}

int main()
{
    const int N = 5;
    const int LIMIT = 0xFFFF;
    pthread_t threads[N];
    int current_number = 2;
    int active_threads = 0;

    while (current_number <= LIMIT)
    {
        if (active_threads < N && current_number <= LIMIT)
        {
            ThreadData *data = malloc(sizeof(ThreadData));
            if (!data)
            {
                perror("Błąd alokacji pamięci");
                exit(1);
            }
            data->number = current_number;

            if (pthread_create(&threads[active_threads], NULL, thread_func, data) != 0)
            {
                perror("Błąd tworzenia wątku");
                free(data);
                exit(1);
            }
            current_number++;
            active_threads++;
        }
        else
        {
            if (active_threads > 0)
            {
                pthread_join(threads[active_threads - 1], NULL);
                active_threads--;
            }
        }
    }

    for (int i = 0; i < active_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Zakończono wyszukiwanie liczb pierwszych do %d\n", LIMIT);
    return 0;
}