#include "SynchronousDispatcher.h"
#include "EventLoop.h"
#include "EventQueue.h"

#include <iostream>
#include <string>
#include <thread>

class Printer : public IConsumer<int, std::string>
{
public:
    Printer() = default;

    void Consume(int id, const std::string& value) override
    {
        std::cout << "Got \"" << value << "\" from queue #" << id << std::endl;
    }
};

int main()
{
    std::cout << "Hello World!" << std::endl;

    EventQueue<int, std::string> queue;
    SynchronousDispatcher<int, std::string> dispatcher;

    EventLoop<int, std::string> loop;

    loop.Execute(dispatcher, queue);

    Printer printer;
    dispatcher.Subscribe(3, &printer);

    int iteration = 0;
    while (true)
    {
        for (int i = 0; i < 10; ++i)
        {
            bool success = queue.Enqueue(i, std::string("Some trash ") + std::to_string(iteration));
            if (!success)
            {
                std::cerr << "Producer's queue is full. Dropped event id: " << i << std::endl;
            }
        }

        ++iteration;

        if (iteration == 200)
        {
            break;
        }
    }

#if 0   // change to 1 to simulate exit with empty queue
    std::this_thread::sleep_for(std::chrono::seconds(1));
    loop.StopProcessing();
#endif  // #ifdef 1

    return 0;
}
