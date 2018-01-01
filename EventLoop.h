#pragma once

#include "EventQueue.h"
#include "IReceiver.h"

#include <atomic>
#include <thread>

template <typename Key, typename Value>
class EventLoop final
{
public:
    EventLoop()
        : m_receiver(nullptr)
        , m_queue(nullptr)
        , m_running(false)
        , m_execThread()
    {
    }

    EventLoop(const EventLoop&) = delete;
    EventLoop& operator=(const EventLoop&) = delete;
    EventLoop(EventLoop&&) = delete;
    EventLoop& operator=(EventLoop&&) = delete;

    ~EventLoop()
    {
        StopProcessing();
        m_execThread.join();
    }

    void Execute(IReceiver<Key, Value>& receiver, EventQueue<Key, Value>& queue)
    {
        const bool wasRunning = m_running.exchange(true);
        if (wasRunning)
        {
            return;
        }

        m_receiver = &receiver;
        m_queue = &queue;

        m_execThread = std::move(std::thread(std::bind(&EventLoop::Process, this)));
    }

    void StopProcessing()
    {
        const bool wasRunning = m_running.exchange(false);
        if (!wasRunning)
        {
            return;
        }

        m_queue->Enqueue({}, {});

        m_receiver = nullptr;
        m_queue = nullptr;
    }

protected:
    void Process()
    {
        while (m_running)
        {
            std::pair<Key, Value> item = m_queue->Dequeue();

            // race: StopProcessing() might reset m_running to false.
            if (m_running)
            {
                m_receiver->Sink(item.first, item.second);
            }
        }
    }

protected:
    IReceiver<Key, Value>* m_receiver;
    EventQueue<Key, Value>* m_queue;

    std::atomic<bool> m_running;
    std::thread m_execThread;
};
