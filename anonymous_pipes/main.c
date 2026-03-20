#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

/*
* Komunikacja mi�dzyprocesowa (IPC, Inter-Process Communication) w systemie Windows umo�liwia wymian� danych mi�dzy procesami. G��wne mechanizmy IPC w Windows to:
*   Potoki (Pipes): Umo�liwiaj� jednokierunkow� lub dwukierunkow� komunikacj�. Dziel� si� na anonimowe (dla proces�w pokrewnych) i nazwane (dla dowolnych proces�w).
*   Pami�� wsp�dzielona (Shared Memory): Procesy dziel� obszar pami�ci do wymiany danych.
*   Komunikaty (Windows Messages): Procesy wysy�aj� komunikaty do okien lub kolejek komunikat�w.
*   Pliki mapowane w pami�ci (Memory-Mapped Files): Umo�liwiaj� dost�p do pliku jak do pami�ci wsp�dzielonej.
*   Gniazda (Sockets): U�ywane do komunikacji sieciowej mi�dzy procesami.
*   Sygna�y i semafory: S�u�� do synchronizacji proces�w.
* 
* W przedstawionym kodzie u�yto anonimowego potoku (Anonymous Pipe) do komunikacji mi�dzy procesem nadrz�dnym a procesami potomnymi (producent i konsument).
* Potok ten jest jednokierunkowy i dzia�a w obr�bie proces�w pokrewnych (np. rodzic-dziecko).
* 
* Opis kodu i jego dzia�ania
* Kod implementuje prost� aplikacj� demonstruj�c� komunikacj� mi�dzyprocesow� za pomoc� anonimowego potoku. Program sk�ada si� z dw�ch r�l:
* 
* Producent: Generuje losowe liczby (trzy warto�ci float), kt�re reprezentuj� d�ugo�ci bok�w tr�jk�ta.
* Konsument (Worker): Odczytuje te liczby z potoku, sprawdza, czy mog� utworzy� tr�jk�t (warunek tr�jk�ta: suma dw�ch bok�w wi�ksza od trzeciego),
* a je�li tak, oblicza pole tr�jk�ta za pomoc� wzoru Herona i wy�wietla wyniki.
* Proces nadrz�dny tworzy potok i uruchamia dwa procesy potomne:
* Producent zapisuje dane do potoku (przekierowanie standardowego wyj�cia na potok).
* Konsument odczytuje dane z potoku (przekierowanie standardowego wej�cia na potok) i przetwarza je.
*/

const size_t N = 1'000;

float random_number()
{
    return (float)abs(rand()) / RAND_MAX * 10;
}

void producer()
{
    srand((unsigned int)time(NULL));
    for (size_t i = 0; i < N; i++)
    {
        float a = random_number();
        float b = random_number();
        float c = random_number();
        printf_s("%f %f %f", a, b, c);
    }
}

void worker()
{
    float a = 0.0f;
    float b = 0.0f;
    float c = 0.0f;

    int triangles = 0;

    for (size_t i = 0; i < N; i++)
    {
        scanf_s("%f %f %f", &a, &b, &c);

        if (a < b + c && b < a + c && c < a + b)
        {
            float p = (a + b + c) / 2;
            float S = sqrtf(p * (p - a) * (p - b) * (p - c));
            printf_s("Triangle: %f %f %f\tField: %f\n", a, b, c, S);
            triangles++;
        }
    }

    printf("%i %zu\n", triangles, N);
}

int main(int argc, char* argv[])
{
    if (2 == argc)
    {
        if ('p' == argv[1][0])
        {
            producer();
            return 0;
        }
        if ('w' == argv[1][0])
        {
            worker();
            return 0;
        }
    }

    HANDLE hReadPipe, hWritePipe;
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

    if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, 0)) {
        printf("B��d tworzenia potoku: %d\n", GetLastError());
        return 1;
    }

    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION piProducer, piConsumer;

    si.hStdOutput = hWritePipe;
    si.hStdInput = hReadPipe;
    si.dwFlags = STARTF_USESTDHANDLES;

    char buff[MAX_PATH];
    sprintf_s(buff, MAX_PATH, "%s %c", argv[0], 'p');
    if (!CreateProcessA(NULL, buff, NULL, NULL, TRUE, 0, NULL, NULL, &si, &piProducer)) {
        printf("B��d uruchamiania producenta: %d\n", GetLastError());
        return 1;
    }

    CloseHandle(hWritePipe);

    sprintf_s(buff, MAX_PATH, "%s %c", argv[0], 'w');
    si.hStdInput = hReadPipe;
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!CreateProcessA(NULL, buff, NULL, NULL, TRUE, 0, NULL, NULL, &si, &piConsumer)) {
        printf("B��d uruchamiania konsumenta: %d\n", GetLastError());
        return 1;
    }

    CloseHandle(hReadPipe);

    WaitForSingleObject(piProducer.hProcess, INFINITE);
    WaitForSingleObject(piConsumer.hProcess, INFINITE);

    CloseHandle(piProducer.hProcess);
    CloseHandle(piProducer.hThread);
    CloseHandle(piConsumer.hProcess);
    CloseHandle(piConsumer.hThread);

    return 0;
}