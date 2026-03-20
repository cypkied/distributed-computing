#include <iostream>
#include <array>
#include <memory>
#include "DataQueue.hpp"
#include "Producer.hpp"
#include "Consumer.hpp"

/*
 * Kompilacja: clang++ main.cpp -pthread -o main
 *
 * Semafor binarny (BinarySemaphore.hpp) zaimplementowano w C++ przy użyciu kanału komunikatów (std::queue<int>),
 * gdzie wartość 1 oznacza otwarty semafor. Synchronizacja jest realizowana za pomocą std::mutex i std::condition_variable.
 * Metody open(), acquire(), try_acquire(), close() i opened() zapewniają bezpieczne zarządzanie dostępem.
 *
 * W zastosowaniu (main.cpp) semafor synchronizuje współdzieloną kolejkę (DataQueue) w scenariuszu producent-konsument.
 * Producent (Producer.hpp) generuje 100000 losowych liczb,
 * a czterej konsumenci (Consumer.hpp) pobierają je, zliczając liczby pierwsze.
 * Program wypisuje statystyki i weryfikuje poprawność (suma elementów = 100000).
 * Kod jest wątkowo bezpieczny i przetestowany.
 */

constexpr size_t N = 100000;

int main()
{
    std::srand(0x777);
    std::shared_ptr<DataQueue<int>> queue_ptr = std::make_shared<DataQueue<int>>();
    Producer producer;
    producer.produce(queue_ptr, N);
    std::array<Consumer, 4> consumer;
    for (size_t i = 0; i < consumer.size(); i++)
    {
        consumer[i].consume(queue_ptr);
    }

    producer.join();
    for (size_t i = 0; i < consumer.size(); i++)
    {
        consumer[i].join();
    }

    size_t counted = 0;
    size_t prime = 0;
    for (size_t i = 0; i < consumer.size(); i++)
    {
        std::cout << i << ": " << consumer[i].number() << '\n';
        counted += consumer[i].number();
        prime += consumer[i].prime();
    }

    std::cout << "Primes: " << prime << '\n';
    std::cout << (counted == N ? "Equal" : "Different") << '\n';

    return 0;
}