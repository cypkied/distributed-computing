#include <iostream>
#include <array>
#include "Store.hpp"
#include "Producer.hpp"
#include "Consumer.hpp"

/*
 * Program implementuje wielowątkowy model producent-konsument w C++.
 *
 * **Opis działania**:
 * - Magazyn (Store) przechowuje towary 10 różnych typów, z maksymalną ilością 255 jednostek na typ.
 * - Producent (Producer) cyklicznie losuje typ towaru i ilość, dodając je do magazynu, jeśli jest miejsce.
 * - Konsumenci (Consumer) losują typ i ilość towaru,
 *   próbując dokonać zakupu; zakup jest realizowany w całości lub częściowo,
 *   w zależności od dostępności.
 * - Wszystkie operacje na magazynie (dodawanie, usuwanie, odczyt) są zsynchronizowane za pomocą std::mutex, co zapobiega warunkom wyścigu.
 * - Wątki działają cyklicznie z losowymi opóźnieniami (konsumenci: 0–999 ms, producent: 1000 ms), co zapewnia dynamiczną symulację.
 *
 * **Przykładowe zastosowanie**:
 * Program uruchamia jeden wątek producenta i 10 wątków konsumentów.
 * Co sekundę wypisuje stan magazynu,
 * pokazując zmiany w ilościach towarów.
 *
 * **Kompilacja**:
 * clang++ -std=c++17 main.cpp -pthread
 */

int main()
{
    std::srand(std::time(nullptr));

    Store store;
    Producer producer(store);
    producer.init();
    std::array<Consumer *, 15> consumer;
    for (int i = 0; i < consumer.size(); i++)
    {
        consumer[i] = new Consumer(store);
        consumer[i]->init();
    }

    while(true){};

    return 0;
}