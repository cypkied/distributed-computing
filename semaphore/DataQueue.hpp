#pragma once
#include <queue>
#include "BinarySemaphore.hpp"

template <typename T>
class DataQueue
{
public:
    void push(const T &data)
    {
        semaphore.acquire();
        queue.push(data);
        semaphore.open();
    }

    T pop()
    {
        semaphore.acquire();
        T data = queue.front();
        queue.pop();
        semaphore.open();
        return data;
    }

    bool empty()
    {
        semaphore.acquire();
        bool is_empty = queue.empty();
        semaphore.open();
        return is_empty;
    }

    void close()
    {
        closed_ = true;
    }

    bool closed()
    {
        semaphore.acquire();
        bool closed_ = this->closed_;
        semaphore.open();
        return closed_;
    }

private:
    std::queue<T> queue;
    BinarySemaphore semaphore;
    bool closed_ = false;
};