#pragma once

#include <cstdint>

namespace trading_engine {

using OrderId = std::uint64_t;
using Price = std::int64_t;      // Positive integer ticks, never floating point.
using Quantity = std::uint64_t;

enum class Side { Buy, Sell };

struct Order {
    OrderId id;
    Side side;
    Price price;
    Quantity quantity;
};

}  // namespace trading_engine

