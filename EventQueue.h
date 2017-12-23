#pragma once

#include <map>
#include <list>
#include <thread>
#include <mutex>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

static const int MaxCapacity = 1000;

template <typename Key, typename Value>
class EventQueue
{
public:
    EventQueue()
    {
    }

    bool Enqueue(Key id, Value value)
    {
        std::lock_guard<std::mutex> lock{mtx};
        if (queues.size() >= MaxCapacity)
        {
            return false;
        }

        queues.push(std::make_pair(id, value));
        cond.notify_one();

        return true;
    }

    std::pair<Key, Value> Dequeue()
    {
        std::unique_lock<std::mutex> pop_guard(mtx);
        cond.wait(pop_guard, [this]() { return !queues.empty(); });

        std::pair<Key, Value> item = queues.front();
        queues.pop();

        return item;
    }

private:
    std::queue<std::pair<Key, Value>> queues;
    std::mutex mtx;
    std::condition_variable cond;
};
