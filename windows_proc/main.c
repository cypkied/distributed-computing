#include <windows.h>
#include <stdio.h>
#include <time.h>
#include "QuadraticFunction.h"

/*
* G³ówn¹ ró¿nic¹ miêdzy tworzeniem programu w C wykorzystuj¹cego wieloprocesowoœæ miêdzy systemem Unix i Windows,
* jest fakt, ¿e w systemach Unix fork() tworzy kopiê procesu macierzystego i domyœlnie operuje na jednym obrazie binarnym (tym z którego zosta³ wywo³any),
* a CreateProcess z WinApi wymaga wskazania obrazu binarnego na jakim bêdzie pracowa³
* 
* Skrótowy opis dzia³ania programu:
*   Program losuje parametry funkcji kwadratowej, a nastêpnie zapisuje ich dzia³ania do plików
* 
* Ka¿dy proces jest nowym bytem, ale ziarna generowania s¹ róŸne poniewa¿ time(NULL) dzia³a z dok³adanoœci¹ do sekundy, co powoduje, ¿e procesy mog³yby mieæ takie samo ziarno
* Wykorzystuje CreateProcessA z WinApi
* Program dzia³a na jednym mainie i charakter dzia³ania zale¿y od liczby podanych argumentów przy wywo³ywaniu
* G³ówny proces oczekuje na zakoñczenie przy pomocy WaitForSingleObject()
* 
* Zwiêkszaj¹c N mo¿na zauwa¿yæ, ¿e main wykorzystuj¹cy podprocesy jest oko³o dwukrotnie szybszy.
* Nie op³aca siê jednak wykorzystywaæ takiego rozwi¹zaniam je¿eli narzut zwi¹zany z tworzeniem podprocesów jest wiêkszy od obliczeñ.
*/

PROCESS_INFORMATION* create_process(char path[MAX_PATH])
{
    STARTUPINFOA si;
    PROCESS_INFORMATION* pi = malloc(sizeof(PROCESS_INFORMATION));

    if (NULL == pi)
    {
        return NULL;
    }

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(pi, sizeof(*pi));

    char command[MAX_PATH] = "";
    sprintf_s(command, MAX_PATH, "%s %s", path, "0");

    if (!CreateProcessA(NULL,
        command,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        pi)
        )
    {
        return NULL;
    }

    return pi;
}

PROCESS_INFORMATION* kill_process(PROCESS_INFORMATION* process)
{
    WaitForSingleObject(process->hProcess, INFINITE);
    CloseHandle(process->hProcess);
    CloseHandle(process->hThread);
    free(process);

    return NULL;
}

void task(size_t N)
{
    DWORD pid = GetCurrentProcessId();
    char buff[MAX_PATH];
    sprintf_s(buff, MAX_PATH, "%u.txt", (unsigned int)pid);

    FILE* file = fopen(buff, "w");
    for (size_t i = 0; i < N; i++)
    {
        QuadraticFunction_t quadratic_function;
        quadratic_function.a = 0.0f;
        while (0.0f == quadratic_function.a)
        {
            quadratic_function.a = (float)rand();
            quadratic_function.a = 0.0f == quadratic_function.a ? 0.0f : (float)rand() / quadratic_function.a;
        }
        quadratic_function.b = (float)rand();
        quadratic_function.c = (float)rand();
        quadratic_function.b = 0.0f == quadratic_function.b ? 0.0f : (float)rand() / quadratic_function.b;
        quadratic_function.c = 0.0f == quadratic_function.c ? 0.0f : (float)rand() / quadratic_function.c;

        ZeroPlacesQF_t zero_places_qf = find_zp_qt(quadratic_function);

        fprintf_qf(quadratic_function, file);
        fprintf_zp_qf(zero_places_qf, file);
    }

    fclose(file);
}

/*
* PS D:\Rozprochy\x64\Debug> Measure-Command { .\Rozprochy.exe }
* Days              : 0
* Hours             : 0
* Minutes           : 1
* Seconds           : 3
* Milliseconds      : 962
* Ticks             : 639625310
* TotalDays         : 0,000740307071759259
* TotalHours        : 0,0177673697222222
* TotalMinutes      : 1,06604218333333
* TotalSeconds      : 63,962531
* TotalMilliseconds : 63962,531
*/

/*int main(int argc, char* argv[])
{
    srand(time(NULL) ^ GetCurrentProcessId());

    const size_t N = 10'000'000;

    if (argc == 2)
    {
        task(N);
        return 0;
    }

    char path[MAX_PATH] = "";
    DWORD length = GetModuleFileNameA(NULL, path, MAX_PATH);

    if (0 == length)
    {
        return 1;
    }

    PROCESS_INFORMATION* proc1 = create_process(path);
    PROCESS_INFORMATION* proc2 = create_process(path);

    if (NULL == proc1 || NULL == proc2)
    {
        puts("Proces nieudany!\n");
        return 1;
    }
    
    proc1 = kill_process(proc1);
    proc2 = kill_process(proc2);

    return 0;
}*/

/*
* PS D:\Rozprochy\x64\Debug> Measure-Command { .\Rozprochy.exe }
* Days              : 0
* Hours             : 0
* Minutes           : 2
* Seconds           : 4
* Milliseconds      : 604
* Ticks             : 1246044040
* TotalDays         : 0,00144218060185185
* TotalHours        : 0,0346123344444444
* TotalMinutes      : 2,07674006666667
* TotalSeconds      : 124,604404
* TotalMilliseconds : 124604,404
*/

int main(int argc, char* argv[])
{
    srand(time(NULL) ^ GetCurrentProcessId());

    const size_t N = 10'000'000;

    task(2 * N);

    return 0;
}