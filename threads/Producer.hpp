#pragma once
#include <ostream>
#include <limits>
#include <thread>
#include <cstdlib>
#include <chrono>
#include "Store.hpp"

class Producer
{
public:
    static constexpr std::chrono::milliseconds SLEEP_TIME = std::chrono::milliseconds(1000);

    Producer(Store &store) : store(store) {}
    ~Producer()
    {
        thread.join();
    }

    void init()
    {
        thread = std::thread(&Producer::produce, this);
    }

    void kill()
    {
        thread.join();
    }

protected:
    void produce()
    {
        while (true)
        {
            Store::type_t type = std::rand() % Store::NO_TYPES;
            Store::type_t new_max_quantity = Store::MAX_OF_TYPES - store[type];
            if (new_max_quantity)
            {
                store.buy(type, std::rand() % new_max_quantity);
            }

            std::cout << store << '\n';
            std::this_thread::sleep_for(Producer::SLEEP_TIME);
        }
    }

private:
    Store &store;
    std::thread thread;
    bool produce_;
};