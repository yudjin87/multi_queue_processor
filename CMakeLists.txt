project(multiqueue)
cmake_minimum_required(VERSION 2.8)

set(SRC_LIST
    EventLoop.h
    EventQueue.h
    fwd.h
    IConsumer.h
    IDispatcher.h
    IReceiver.h
    main.cpp
    MultiQueueFacade.h
    MultiQueueProcessor.h
    SynchronousDispatcher.h
)

add_executable(${PROJECT_NAME} ${SRC_LIST})

