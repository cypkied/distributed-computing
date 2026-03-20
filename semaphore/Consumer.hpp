#pragma once
#include <iostream>
#include <thread>
#include <memory>
#include "DataQueue.hpp"

class Consumer
{
public:
    void consume(const std::shared_ptr<DataQueue<int>> &queue_ptr)
    {
        this->queue_ptr = queue_ptr;
        this->number_ = 0;
        thread = std::thread(&Consumer::consume_impl, this);
    }

    void join()
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }

    const int prime() const
    {
        return prime_;
    }

    const int number() const
    {
        return number_;
    }

protected:
    bool is_prime(const int n) const
    {
        if (n < 2)
        {
            return false;
        }
        if (n == 2)
        {
            return true;
        }
        for (int i = 2; i * i <= n; i += 2)
        {
            if (n % i == 0)
            {
                return false;
            }
        }

        return true;
    }

    void consume_impl()
    {
        while (!queue_ptr->closed())
        {
            while (!queue_ptr->empty())
            {
                const int n = queue_ptr->pop();
                prime_ += is_prime(n);
                number_++;
            }
        }
    }

private:
    std::thread thread;
    std::shared_ptr<DataQueue<int>> queue_ptr;
    size_t prime_ = 0;
    size_t number_ = 0;
};