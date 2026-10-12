#pragma once

#include "trading_engine/order_book.hpp"

#include <vector>

namespace trading_engine {

enum class OrderType { Limit, Market };

struct IncomingOrder {
    OrderId id;
    Side side;
    OrderType type;
    Price price;        // Required for limit orders; ignored for market orders.
    Quantity quantity;
};

struct Trade {
    OrderId incoming_order_id;
    OrderId resting_order_id;
    Price price;        // Resting order's price.
    Quantity quantity;
};

enum class SubmitStatus {
    Accepted,
    DuplicateActiveId,
    InvalidPrice,
    InvalidQuantity,
    QuantityOverflow,
};

struct SubmitResult {
    SubmitStatus status;
    std::vector<Trade> trades;
    Quantity rested_quantity = 0;
    Quantity expired_quantity = 0;  // Unfilled market quantity.
};

class MatchingEngine {
public:
    SubmitResult submit(const IncomingOrder& incoming);
    bool cancel(OrderId id);
    [[nodiscard]] const OrderBook& book() const noexcept;

private:
    OrderBook book_;
};

}  // namespace trading_engine

