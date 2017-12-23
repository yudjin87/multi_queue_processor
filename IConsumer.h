#pragma once

template <typename Key, typename Value>
class IConsumer
{
public:
    IConsumer(const IConsumer&) = delete;
    IConsumer operator&(const IConsumer&) = delete;
    virtual ~IConsumer() = default;

    virtual void Consume(Key id, const Value& value) = 0;

protected:
    IConsumer() = default;
};
