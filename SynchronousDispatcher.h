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

        std::weak_ptr<Consumer> consumer = consumerIter->second;
        lock.unlock();

        // It's a compromise betweed dead-lock (Consume might call unsubscribe)
        // and life-time race (right now someone might call unsubscribe)
        //
        // I've chosen 1 option, as it's implemented in boost, and it's a client
        // responsibility to synchronize life-time.
        //
        // Also, it's MT best practice - don't hold mutex while calling 3rd party code
        
        if (auto consumerSPtr = consumer.lock())
        {
            consumerSPtr->Consume(id, value);
        }
    }

    // Having shared ptr here is not good desicion, since it forces client to have all customer be shared_ptr
    // However, shared_ptr has been chosen in sake of simplicity and error-proof
    // Proper solution would be implementation of additional layer of "weak connections", which have shared/weak ptr
    // semantic, but doesn' force consumer to be std::shared_ptr<>
    void Subscribe(Key id, std::shared_ptr<Consumer> consumer) override
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
    std::unordered_map<Key, std::weak_ptr<Consumer>> consumers;
    std::mutex mtx;
};
