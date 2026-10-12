#include "trading_engine/matching_engine.hpp"

namespace trading_engine {

SubmitResult MatchingEngine::submit(const IncomingOrder& incoming) {
    if (incoming.quantity == 0) return {SubmitStatus::InvalidQuantity, {}, 0, 0};
    if (incoming.type == OrderType::Limit && incoming.price <= 0) {
        return {SubmitStatus::InvalidPrice, {}, 0, 0};
    }
    if (book_.contains(incoming.id)) return {SubmitStatus::DuplicateActiveId, {}, 0, 0};

    // Check the maximum possible resting quantity before executing any trades.
    if (incoming.type == OrderType::Limit &&
        book_.would_overflow(incoming.side, incoming.price, incoming.quantity)) {
        return {SubmitStatus::QuantityOverflow, {}, 0, 0};
    }

    SubmitResult result{SubmitStatus::Accepted, {}, 0, 0};
    Quantity remaining = incoming.quantity;
    const Side resting_side = incoming.side == Side::Buy ? Side::Sell : Side::Buy;

    while (remaining > 0) {
        const auto best = resting_side == Side::Buy ? book_.best_bid() : book_.best_ask();
        if (!best) break;
        if (incoming.type == OrderType::Limit) {
            if (incoming.side == Side::Buy && incoming.price < best->price) break;
            if (incoming.side == Side::Sell && incoming.price > best->price) break;
        }

        const auto fill = book_.consume_best(resting_side, remaining);
        result.trades.push_back({incoming.id, fill.resting_order_id, fill.price, fill.quantity});
        remaining -= fill.quantity;
    }

    if (remaining > 0 && incoming.type == OrderType::Limit) {
        // The earlier overflow check and ID check guarantee this can rest.
        book_.add_limit({incoming.id, incoming.side, incoming.price, remaining});
        result.rested_quantity = remaining;
    } else {
        result.expired_quantity = remaining;
    }
    return result;
}

bool MatchingEngine::cancel(OrderId id) { return book_.cancel(id); }

const OrderBook& MatchingEngine::book() const noexcept { return book_; }

}  // namespace trading_engine

