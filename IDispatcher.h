#pragma once

#include "fwd.h"

template <typename Key, typename Value>
class IDispatcher
{
public:
    using Consumer = IConsumer<Key, Value>;

public:
    IDispatcher(const IDispatcher&) = delete;
    IDispatcher operator&(const IDispatcher&) = delete;
    virtual ~IDispatcher() = default;

    virtual void Subscribe(Key id, Consumer* consumer) = 0;
    virtual void Unsubscribe(Key id) = 0;

protected:
    IDispatcher() = default;
};
