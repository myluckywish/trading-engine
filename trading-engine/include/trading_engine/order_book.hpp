#pragma once

#include "trading_engine/order.hpp"

#include <cstddef>
#include <functional>
#include <list>
#include <map>
#include <optional>
#include <unordered_map>

namespace trading_engine {

enum class AddResult {
    Added,
    DuplicateId,
    InvalidPrice,
    InvalidQuantity,
    QuantityOverflow,
};

struct LevelView {
    Price price;
    Quantity total_quantity;
    std::size_t order_count;
    OrderId oldest_order_id;
};

class MatchingEngine;

// Stores resting limit orders; MatchingEngine controls execution.
class OrderBook {
public:
    AddResult add_limit(const Order& order);
    bool cancel(OrderId id);

    [[nodiscard]] std::optional<LevelView> best_bid() const;
    [[nodiscard]] std::optional<LevelView> best_ask() const;
    [[nodiscard]] std::size_t order_count() const noexcept;
    [[nodiscard]] bool contains(OrderId id) const;

private:
    friend class MatchingEngine;

    struct Fill {
        OrderId resting_order_id;
        Price price;
        Quantity quantity;
    };

    [[nodiscard]] bool would_overflow(Side side, Price price, Quantity quantity) const;
    Fill consume_best(Side resting_side, Quantity max_quantity);

    struct PriceLevel {
        std::list<Order> orders;  // Arrival order is FIFO within one price level.
        Quantity total_quantity = 0;
    };

    struct Location {
        Side side;
        Price price;
        std::list<Order>::iterator order;
    };

    std::map<Price, PriceLevel, std::greater<Price>> bids_;
    std::map<Price, PriceLevel> asks_;
    std::unordered_map<OrderId, Location> locations_;
};

}  // namespace trading_engine

