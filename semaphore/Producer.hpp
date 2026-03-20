#pragma once
#include <random>
#include <thread>
#include <memory>
#include "DataQueue.hpp"

class Producer
{
public:
    void produce(const std::shared_ptr<DataQueue<int>> &queue_ptr, const size_t number)
    {
        this->queue_ptr = queue_ptr;
        this->number = number;
        thread = std::thread(&Producer::produce_impl, this);
    }

    void join()
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }

protected:
    void produce_impl()
    {
        for (size_t i = 0; i < number; i++)
        {
            queue_ptr->push(std::rand());
        }
        queue_ptr->close();
    }

private:
    std::thread thread;
    std::shared_ptr<DataQueue<int>> queue_ptr;
    size_t number;
};