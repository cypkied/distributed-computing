#pragma once
#include <ostream>
#include <limits>
#include <thread>
#include <cstdlib>
#include <chrono>
#include "Store.hpp"

class Consumer
{
public:
    Consumer(Store &store) : store(store) {}
    ~Consumer()
    {
        thread.join();
    }

    void init()
    {
        thread = std::thread(&Consumer::consume, this);
    }

    void kill()
    {
        thread.join();
    }

protected:
    void consume()
    {
        Store::type_t current_type = std::rand() % Store::NO_TYPES;
        Store::type_t current_quantity = std::rand() % Store::MAX_OF_TYPES;
        while (true)
        {
            current_quantity -= store.sell(current_type, current_quantity);
            if (0 == current_quantity)
            {
                current_type = std::rand() % Store::NO_TYPES;
                current_quantity = std::rand() % Store::MAX_OF_TYPES;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 1000));
        }
    }

private:
    Store &store;
    std::thread thread;
};