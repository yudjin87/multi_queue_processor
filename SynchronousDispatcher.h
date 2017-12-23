#pragma once

#include "IConsumer.h"
#include "IDispatcher.h"
#include "IReceiver.h"

#include <cassert>
#include <iostream>
#include <unordered_map>
#include <mutex>

template <typename Key, typename Value>
class SynchronousDispatcher final : public IDispatcher<Key, Value>, public IReceiver<Key, Value>
{
public:
    using Consumer = IConsumer<Key, Value>;

public:
    SynchronousDispatcher()
    {
    }

    void Sink(Key id, const Value& value) override
    {
        std::unique_lock<std::mutex> lock(mtx);

        auto consumerIter = consumers.find(id);
        if (consumerIter == consumers.end())
        {
            std::cout << "SynchronousDispatcher: no clients for event #" << id << std::endl;
            return;
        }

        lock.unlock();

        // It's a compromise betweed dead-lock (Consume might call unsubscribe)
        // and life-time race (right now someone might call unsubscribe and kill
        // consumer)
        //
        // I've chosen 1 option, as it's implemented in boost, and it's a client
        // responsibility to synchronize life-time.
        //
        // Also, it's MT best practice - don't hold mutex while calling 3rd party code

        Consumer* consumer = consumerIter->second;
        consumer->Consume(id, value);
    }

    void Subscribe(Key id, Consumer* consumer) override
    {
        assert(consumer != nullptr);

        std::lock_guard<std::mutex> lock{mtx};
        auto iter = consumers.find(id);
        if (iter == consumers.end())
        {
            consumers.insert(std::make_pair(id, consumer));
        }
    }

    void Unsubscribe(Key id) override
    {
        std::lock_guard<std::mutex> lock{mtx};
        auto iter = consumers.find(id);
        if (iter != consumers.end())
        {
            consumers.erase(id);
        }
    }

private:
    std::unordered_map<Key, Consumer*> consumers;
    std::mutex mtx;
};
