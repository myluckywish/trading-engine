#include "trading_engine/order_book.hpp"

#include <iterator>
#include <limits>

namespace trading_engine {

AddResult OrderBook::add_limit(const Order& order) {
    if (order.price <= 0) return AddResult::InvalidPrice;
    if (order.quantity == 0) return AddResult::InvalidQuantity;
    if (locations_.contains(order.id)) return AddResult::DuplicateId;

    if (order.side == Side::Buy) {
        auto [level_it, inserted] = bids_.try_emplace(order.price);
        auto& level = level_it->second;
        if (order.quantity > std::numeric_limits<Quantity>::max() - level.total_quantity) {
            if (inserted) bids_.erase(level_it);
            return AddResult::QuantityOverflow;
        }
        level.orders.push_back(order);
        auto order_it = std::prev(level.orders.end());
        locations_.emplace(order.id, Location{order.side, order.price, order_it});
        level.total_quantity += order.quantity;
    } else {
        auto [level_it, inserted] = asks_.try_emplace(order.price);
        auto& level = level_it->second;
        if (order.quantity > std::numeric_limits<Quantity>::max() - level.total_quantity) {
            if (inserted) asks_.erase(level_it);
            return AddResult::QuantityOverflow;
        }
        level.orders.push_back(order);
        auto order_it = std::prev(level.orders.end());
        locations_.emplace(order.id, Location{order.side, order.price, order_it});
        level.total_quantity += order.quantity;
    }
    return AddResult::Added;
}

bool OrderBook::cancel(OrderId id) {
    auto location_it = locations_.find(id);
    if (location_it == locations_.end()) return false;

    const auto& location = location_it->second;
    if (location.side == Side::Buy) {
        auto level_it = bids_.find(location.price);
        auto& level = level_it->second;
        level.total_quantity -= location.order->quantity;
        level.orders.erase(location.order);
        if (level.orders.empty()) bids_.erase(level_it);
    } else {
        auto level_it = asks_.find(location.price);
        auto& level = level_it->second;
        level.total_quantity -= location.order->quantity;
        level.orders.erase(location.order);
        if (level.orders.empty()) asks_.erase(level_it);
    }
    locations_.erase(location_it);
    return true;
}

std::optional<LevelView> OrderBook::best_bid() const {
    if (bids_.empty()) return std::nullopt;
    const auto& [price, level] = *bids_.begin();
    return LevelView{price, level.total_quantity, level.orders.size(), level.orders.front().id};
}

std::optional<LevelView> OrderBook::best_ask() const {
    if (asks_.empty()) return std::nullopt;
    const auto& [price, level] = *asks_.begin();
    return LevelView{price, level.total_quantity, level.orders.size(), level.orders.front().id};
}

std::size_t OrderBook::order_count() const noexcept { return locations_.size(); }

}  // namespace trading_engine

