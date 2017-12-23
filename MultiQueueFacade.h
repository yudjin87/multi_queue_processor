#pragma once

#include "SynchronousDispatcher.h"
#include "EventLoop.h"
#include "EventQueue.h"

template <typename Key, typename Value>
class MultiQueueFacade
{
public:
    MultiQueueFacade()
        : queue()
        , dispatcher()
        , loop()
    {
    }

    ~MultiQueueFacade()
    {
        StopProcessing();
    }

    void Run()
    {
        loop.Execute(dispatcher, queue);
    }

    void StopProcessing()
    {
        loop.StopProcessing();
    }

    void Subscribe(Key id, IConsumer<Key, Value>* consumer)
    {
        dispatcher.Subscribe(id, consumer);
    }

    void Unsubscribe(Key id)
    {
        dispatcher.Unsubscribe(id);
    }

    bool Enqueue(Key id, Value value)
    {
        return queue.Enqueue(id, value);
    }

private:
    EventQueue<Key, Value> queue;
    SynchronousDispatcher<Key, Value> dispatcher;
    EventLoop<Key, Value> loop;
};
