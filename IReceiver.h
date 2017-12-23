#pragma once

template <typename Key, typename Value>
class IReceiver
{
public:
    IReceiver(const IReceiver&) = delete;
    IReceiver operator&(const IReceiver&) = delete;
    virtual ~IReceiver() = default;

    virtual void Sink(Key id, const Value& value) = 0;

protected:
    IReceiver() = default;
};
