project(multiqueue)
cmake_minimum_required(VERSION 2.8)

set(SRC_LIST
    main.cpp
    MultiQueueProcessor.h)

add_executable(${PROJECT_NAME} ${SRC_LIST})

