#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string.h>

#pragma comment(lib, "Ws2_32.lib")

/*
* Opis dzia³ania:Program sk³ada siê z dwóch g³ównych komponentów:
* 
* Producent - generuje losowe trójki liczb (a, b, c) reprezentuj¹ce potencjalne boki trójk¹ta i wysy³a je za pomoc¹ gniazda UDP na lokalny adres (127.0.0.1:8080).
* 
* Konsument - odbiera dane, sprawdza, czy trójka liczb spe³nia warunek istnienia trójk¹ta (suma dwóch boków wiêksza od trzeciego),
* a nastêpnie oblicza pole metod¹ Herona dla prawid³owych trójk¹tów.
* 
* Program uruchamia siê w dwóch trybach:
* Bez argumentów: automatycznie tworzy dwa procesy (producent i konsument).
* Z argumentem 'p' lub 'w': uruchamia odpowiednio producenta lub konsumenta.
*/

const size_t N = 1000;
const char* SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 8080;

typedef struct {
    float a, b, c;
} TriangleData;

float random_number() {
    return (float)abs(rand()) / RAND_MAX * 10;
}

void producer() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("B³¹d inicjalizacji Winsock: %d\n", WSAGetLastError());
        return;
    }

    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == INVALID_SOCKET) {
        printf("B³¹d tworzenia gniazda: %d\n", WSAGetLastError());
        WSACleanup();
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("B³¹d bindowania: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return;
    }

    struct sockaddr_in client_addr;
    int client_len = sizeof(client_addr);
    char buffer[1];
    if (recvfrom(sockfd, buffer, 1, 0, (struct sockaddr*)&client_addr, &client_len) == SOCKET_ERROR) {
        printf("B³¹d odbioru: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return;
    }

    srand((unsigned int)time(NULL));
    for (size_t i = 0; i < N; i++) {
        TriangleData data;
        data.a = random_number();
        data.b = random_number();
        data.c = random_number();
        if (sendto(sockfd, (char*)&data, sizeof(TriangleData), 0,
            (struct sockaddr*)&client_addr, client_len) == SOCKET_ERROR) {
            printf("B³¹d wysy³ania: %d\n", WSAGetLastError());
        }
        printf("Wys³ano: %f %f %f\n", data.a, data.b, data.c);
    }

    closesocket(sockfd);
    WSACleanup();
}

void worker() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("B³¹d inicjalizacji Winsock: %d\n", WSAGetLastError());
        return;
    }

    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == INVALID_SOCKET) {
        printf("B³¹d tworzenia gniazda: %d\n", WSAGetLastError());
        WSACleanup();
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    server_addr.sin_port = htons(SERVER_PORT);

    char buffer[1] = { 0 };
    if (sendto(sockfd, buffer, 1, 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("B³¹d wysy³ania: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return;
    }

    TriangleData data;
    int triangles = 0;
    struct sockaddr_in from_addr;
    int from_len = sizeof(from_addr);

    for (size_t i = 0; i < N; i++) {
        if (recvfrom(sockfd, (char*)&data, sizeof(TriangleData), 0,
            (struct sockaddr*)&from_addr, &from_len) == SOCKET_ERROR) {
            printf("B³¹d odbioru: %d\n", WSAGetLastError());
            continue;
        }

        float a = data.a, b = data.b, c = data.c;
        if (a < b + c && b < a + c && c < a + b) {
            float p = (a + b + c) / 2;
            float S = sqrtf(p * (p - a) * (p - b) * (p - c));
            printf("Triangle: %f %f %f\tField: %f\n", a, b, c, S);
            triangles++;
        }
    }

    printf("%i %zu\n", triangles, N);

    closesocket(sockfd);
    WSACleanup();
}

int main(int argc, char* argv[]) {
    if (argc == 2) {
        if (argv[1][0] == 'p') {
            producer();
            return 0;
        }
        if (argv[1][0] == 'w') {
            worker();
            return 0;
        }
    }

    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION piProducer, piConsumer;
    char buff[MAX_PATH];

    sprintf_s(buff, MAX_PATH, "%s p", argv[0]);
    if (!CreateProcessA(NULL, buff, NULL, NULL, FALSE, 0, NULL, NULL, &si, &piProducer)) {
        printf("B³¹d uruchamiania producenta: %d\n", GetLastError());
        return 1;
    }

    Sleep(100);

    sprintf_s(buff, MAX_PATH, "%s w", argv[0]);
    if (!CreateProcessA(NULL, buff, NULL, NULL, FALSE, 0, NULL, NULL, &si, &piConsumer)) {
        printf("B³¹d uruchamiania konsumenta: %d\n", GetLastError());
        return 1;
    }

    WaitForSingleObject(piProducer.hProcess, INFINITE);
    WaitForSingleObject(piConsumer.hProcess, INFINITE);

    CloseHandle(piProducer.hProcess);
    CloseHandle(piProducer.hThread);
    CloseHandle(piConsumer.hProcess);
    CloseHandle(piConsumer.hThread);

    return 0;
}