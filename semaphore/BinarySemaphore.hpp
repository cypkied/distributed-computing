#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

class BinarySemaphore
{
public:
    BinarySemaphore()
    {
        channel.push(1);
    }

    void open()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        channel.push(1);
        cond_.notify_one();
    }

    void close()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        while (!channel.empty())
        {
            channel.pop();
        }
        cond_.notify_one();
    }

    bool try_acquire()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        if (!channel.empty())
        {
            channel.pop();
            return true;
        }
        return false;
    }

    void acquire()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this]
                   { return !channel.empty(); });
        channel.pop();
    }

    bool opened() const
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return !channel.empty();
    }

private:
    mutable std::mutex mutex_;
    std::condition_variable cond_;
    std::queue<int> channel;
};