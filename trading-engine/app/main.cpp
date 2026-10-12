#include "trading_engine/matching_engine.hpp"

#include <iostream>

int main() {
    using namespace trading_engine;

    MatchingEngine engine;
    engine.submit({1, Side::Sell, OrderType::Limit, 101, 4});
    engine.submit({2, Side::Sell, OrderType::Limit, 101, 6});
    const auto result = engine.submit({3, Side::Buy, OrderType::Limit, 101, 7});

    for (const auto& trade : result.trades) {
        std::cout << "Trade: buy " << trade.incoming_order_id << " vs sell "
                  << trade.resting_order_id << ", " << trade.quantity << " units at "
                  << trade.price << " ticks\n";
    }
    std::cout << "Remaining ask: " << engine.book().best_ask()->total_quantity
              << " units at " << engine.book().best_ask()->price << " ticks\n";
}

