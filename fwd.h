#pragma once

#include <memory>

template <typename Key, typename Value>
class IConsumer;

template <typename Key, typename Value>
using IConsumerSPtr = std::shared_ptr<IConsumer<Key, Value>>;
